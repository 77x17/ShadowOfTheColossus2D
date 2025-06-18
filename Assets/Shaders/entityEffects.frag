uniform sampler2D texture;
uniform float invincibleAmount; // 0 = bình thường, 1 = mờ
uniform float flashAmount;      // 0 = bình thường, 1 = hiệu ứng tối đa
uniform bool  useBlackFlash;    // false = flash trắng, true = flash đen

void main() {
    gl_FragColor = vec4(0.0); // Clear mặc định (fix lỗi một số card driver)
    
    vec4 texColor = texture2D(texture, gl_TexCoord[0].xy);
    
    // Áp hiệu ứng invincible: giảm alpha
    texColor.a *= mix(1.0, 0.0, invincibleAmount);

    // Chọn màu flash: trắng hoặc đen
    vec4 flashColor = vec4(1.0, 1.0, 1.0, texColor.a); // trắng mặc định
    if (useBlackFlash)
        flashColor = vec4(0.0, 0.0, 0.0, texColor.a); // hoặc đen

    // Trộn giữa màu gốc và màu flash theo flashAmount
    gl_FragColor = mix(texColor, flashColor, flashAmount);
}
