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

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
  fragColor = vec4(0.7, 0.7, 0.7,1.0);
}

void main()
{
   mainImage(fragColor, gl_FragCoord.xy);
}
