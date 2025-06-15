uniform sampler2D texture;
uniform float invincibleAmount; // 0 = bình thường, 1 = mờ
uniform float flashAmount;      // 0 = bình thường, 1 = trắng xoá

void main() {
    gl_FragColor = vec4(0.0); // Clear mặc định (fix lỗi một số card driver)
    
    vec4 texColor = texture2D(texture, gl_TexCoord[0].xy);
    
    // Áp hiệu ứng invincible: giảm alpha
    texColor.a *= mix(1.0, 0.0, invincibleAmount); // 0.4 = 40% trong suốt\

    // Áp hiệu ứng flash trắng (knockback)
    vec4 white = vec4(1.0, 1.0, 1.0, texColor.a);
    gl_FragColor = mix(texColor, white, flashAmount);
}
