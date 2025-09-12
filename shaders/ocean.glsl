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

//const vec3 skyColor1 = vec3(0.3, 0.6, 0.9);
//const vec3 skyColor2 = vec3(0.529, 0.808, 0.922);
//const vec3 sunColor = vec3(1.0, 0.9, 0.7);

float wave(vec2 p, float time)
{
  float h = 0.0;
  h += u_waveHeight * 0.5 * sin(p.x * 1.0 + time * u_waveSpeed);
  h += u_waveHeight * 0.3 * sin(p.x * 2.0 + p.y * 0.5 + time * u_waveSpeed * 1.3);
  return h;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
  vec2 uv = (fragCoord.xy - 0.5 * u_resolution.xy) / u_resolution.y;

  float waveVal = wave(uv * 5.0, u_time);
    
  vec3 waveColor = u_color + waveVal * 0.2;
  
  fragColor = vec4(waveColor,1.0);
}

void main()
{
  mainImage(fragColor, gl_FragCoord.xy);
}

