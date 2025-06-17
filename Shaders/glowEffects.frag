// glow_shader.frag
uniform sampler2D texture;
uniform vec4 glowColor;

void main() {
    vec4 texColor = texture2D(texture, gl_TexCoord[0].xy);
    float alpha = texColor.a;

    // Ánh sáng mềm
    vec3 glow = glowColor.rgb * alpha;

    gl_FragColor = vec4(texColor.rgb + glow * 0.5, alpha);
}
