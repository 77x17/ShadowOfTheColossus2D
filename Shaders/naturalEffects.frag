uniform sampler2D texture;
uniform vec2 lightPosition; // relative [0,1]
uniform float aspectRatio;  // tỷ lệ khung hình (width/height)

// Darkness uniforms
uniform float darkness;     // 0 = sáng hoàn toàn, 1 = tối đen
uniform float lightRadius;  // bán kính ánh sáng (0.0 - 1.0)

// Fog uniforms
uniform float fogDensity;   // 0 = không có sương mù, 1 = sương mù dày đặc
uniform float clearRadius;  // bán kính vùng không có sương mù (0.0 - 1.0)
uniform vec3 fogColor;      // màu sắc sương mù

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, uv);

    // Sửa lại tọa độ Y
    vec2 correctedPos = vec2(lightPosition.x, 1.0 - lightPosition.y);
    
    // Điều chỉnh tọa độ để tạo hình tròn hoàn hảo
    vec2 adjustedUV = uv;
    vec2 adjustedPos = correctedPos;
    
    if (aspectRatio > 1.0) {
        // Màn hình rộng - nén tọa độ X
        adjustedUV.x *= aspectRatio;
        adjustedPos.x *= aspectRatio;
    } else {
        // Màn hình cao - nén tọa độ Y
        adjustedUV.y /= aspectRatio;
        adjustedPos.y /= aspectRatio;
    }
    
    // Tính khoảng cách từ pixel hiện tại đến player
    float dist = distance(adjustedUV, adjustedPos);
    
    // === ÁP DỤNG HIỆU ỨNG DARKNESS ===
    if (darkness > 0.01) {
        float light = 1.0 - smoothstep(0.0, lightRadius, dist);
        float finalDarkness = mix(darkness, 0.0, light);
        color.rgb *= (1.0 - finalDarkness);
    }
    
    // === ÁP DỤNG HIỆU ỨNG FOG ===
    if (fogDensity > 0.01) {
        float clearArea = 1.0 - smoothstep(0.0, clearRadius, dist);
        float finalFogDensity = mix(0.0, fogDensity, 1.0 - clearArea);
        color.rgb = mix(color.rgb, fogColor, finalFogDensity);
    }

    gl_FragColor = color;
}