#shader vertex
#version 330

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;

out vec3 theColor;

void main()
{
  gl_Position = vec4(position, 1.0);
  theColor = vertexColor;
}

#shader fragment
#version 430

out vec4 fragColor;

uniform vec2 u_resolution;
uniform float u_time;

uniform vec3 u_waterColor;
uniform float u_waveSpeed;
uniform float u_waveHeight;
uniform float u_sunHeight;
//float u_sunHeight = 0.8f;
float u_fresnelPower = 2.0f;
float u_reflectionStrength = 0.8f;
uniform vec3 u_skyColor1;
uniform vec3 u_skyColor2;

uniform vec3 u_sunColor;

float wave(vec2 p, float time)
{
  float h = 0.0;
  h += u_waveHeight * 0.5 * sin(p.x * 1.0 + time * u_waveSpeed);
  h += u_waveHeight * 0.3 * sin(p.x * 2.0 + p.y * 0.5 + time * u_waveSpeed * 1.3);
  h += u_waveHeight * 0.2 * sin(p.x * 0.8 + p.y * 1.2 + time * u_waveSpeed * 0.7);
  h += u_waveHeight * 0.1 * sin(p.x * 3.0 + p.y * 2.0 + time * u_waveSpeed + 2.1);
  return h;
}

vec3 getNormal(vec2 p, float time)
{
  float eps = 0.1;
  float h = wave(p, time);
  float hx = wave(p + vec2(eps, 0.0), time);
  float hy = wave(p + vec2(0.0, eps), time);

  vec3 dx = vec3(eps, hx - h, 0.0);
  vec3 dy = vec3(0.0, hy - h, eps);

  return normalize(cross(dx, dy));
}

vec3 getSkyColor(vec3 rayDir, vec2 uv)
{
  vec3 sunDir = normalize(vec3(0.0, u_sunHeight, 1.0));
  float sunDot = max(dot(rayDir, sunDir), 0.0);

  vec3 sky = mix(u_skyColor1, u_skyColor2, max(0.0, rayDir.y));
  sky += u_sunColor * pow(sunDot, 100.0);
  sky += u_sunColor * 0.3 * pow(sunDot, 30.0);

  return sky;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
  vec2 uv = (fragCoord.xy - 0.5 * u_resolution.xy) / u_resolution.y;

  vec3 rayOrigin = vec3(0.0, 3.0, -2.0);
  vec3 rayDir = normalize(vec3(uv.x, uv.y - 0.2, 1.0));

  vec3 color = vec3(0.0);

  if (rayDir.y < 0.0)
    {
      float t = -rayOrigin.y / rayDir.y;
      vec3 hitPoint = rayOrigin + t * rayDir;
      hitPoint.y = wave(hitPoint.xz, u_time);
      
      vec3 normal = getNormal(hitPoint.xz, u_time);
      vec3 sunDir = normalize(vec3(0.0, 0.8, 1.0));
      float diffuse = max(0.0, dot(normal, sunDir));

      vec3 reflectDir = reflect(rayDir, normal);
      vec3 reflection = getSkyColor(reflectDir, uv);

      float fresnel = pow(1.0 - max(0.0, dot(-rayDir, normal)), u_fresnelPower);
      color = mix(u_waterColor +  diffuse * 0.3, reflection, fresnel * u_reflectionStrength); 
    }
  else
    {
      color = getSkyColor(rayDir, uv);
    }
    
  fragColor = vec4(color,1.0);
}

void main()
{
  mainImage(fragColor, gl_FragCoord.xy);
}

