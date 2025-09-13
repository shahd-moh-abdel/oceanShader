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
uniform vec3 u_color;
uniform float u_waveSpeed;
uniform float u_waveHeight;
//const sunHeight;

uniform vec2 u_resolution;
uniform float u_time;

const vec3 skyColor1 = vec3(0.3, 0.6, 0.9);
const vec3 skyColor2 = vec3(0.529, 0.808, 0.922);
//const vec3 sunColor = vec3(1.0, 0.9, 0.7);

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

vec3 getSkyColor(vec3 rayDir)
{
  vec3 sunDir = normalize(vec3(0.0, 0.8, 1.0));
  float sunDot = max(dot(rayDir, sunDir), 0.0);

  vec3 sky = mix(skyColor1, skyColor2, max(0.0, rayDir.y));
  sky += vec3(1.0, 0.9, 0.7) * pow(sunDot, 100.0);
  sky += vec3(1.0, 0.9, 0.7) * 0.3 * pow(sunDot, 30.0);

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
      vec3 reflection = getSkyColor(reflectDir);

      float fresnel = pow(1.0 - max(0.0, dot(-rayDir, normal)), 2.0);
      color = mix(vec3(0.1, 0.3, 0.6) + diffuse * 0.3, reflection, fresnel * 0.8);
    }
  else
    {
      color = getSkyColor(rayDir);
    }
  
  // float waveVal = wave(uv * 5.0, u_time);
  //vec3 waveColor = u_color + waveVal * 0.2;
  
  fragColor = vec4(color,1.0);
}

void main()
{
  mainImage(fragColor, gl_FragCoord.xy);
}
