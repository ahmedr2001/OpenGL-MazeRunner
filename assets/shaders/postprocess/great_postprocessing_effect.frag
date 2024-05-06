#version 330

uniform sampler2D tex; // Input texture

in vec2 tex_coord;
out vec4 frag_color;

uniform float edgeWidth = 1; // Width of the edge lines
uniform vec4 edgeColor = vec4(1.0, 1.0, 1.0, 1.0); // Color of the edge lines

void main() {
  // Sample the original texture color
  vec4 baseColor = texture(tex, tex_coord);

  // Calculate distance from the edges (0 at center, 1 at edges)
  float distanceX = abs(tex_coord.x - 0.5);
  float distanceY = abs(tex_coord.y - 0.5);

  // Combine distances for a smoother falloff
  float combinedDistance = smoothstep(0.0, edgeWidth, max(distanceX, distanceY));

  // Mix original color with edge color based on distance
  frag_color = mix(baseColor, edgeColor, combinedDistance);
}
