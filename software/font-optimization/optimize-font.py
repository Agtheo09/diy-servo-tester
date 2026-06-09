import re

def optimize_gfx_font(input_file, output_file, new_font_name, start_char, end_char):
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            text = f.read()

        bitmap_match = re.search(r'const uint8_t\s+\w+Bitmaps\[\]\s+PROGMEM\s*=\s*\{(.*?)\};', text, re.DOTALL)
        if not bitmap_match:
            print(f"Could not find bitmap array in {input_file}")
            return
        bitmap_data = bitmap_match.group(1)
        bitmaps = [int(x.strip(), 16) for x in bitmap_data.split(',') if x.strip()]

        # Extract glyph metrics array
        glyph_match = re.search(r'const GFXglyph\s+\w+Glyphs\[\]\s+PROGMEM\s*=\s*\{(.*?)\};', text, re.DOTALL)
        if not glyph_match:
            print(f"Could not find glyph array in {input_file}")
            return
        glyph_data = glyph_match.group(1)
        
        glyph_lines = re.findall(r'\{\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*,\s*(-?\d+)\s*,\s*(-?\d+)\s*,\s*(-?\d+)\s*\}', glyph_data)
        glyphs = [[int(n) for n in line] for line in glyph_lines]

        orig_start = 32
        start_idx = start_char - orig_start
        end_idx = end_char - orig_start
        
        if start_idx < 0 or end_idx >= len(glyphs):
            print(f"Requested range is out of bounds for {input_file}")
            return
            
        kept_glyphs = glyphs[start_idx:end_idx+1]
        
        first_offset = kept_glyphs[0][0]
        if end_idx + 1 < len(glyphs):
            last_offset = glyphs[end_idx + 1][0]
        else:
            last_offset = len(bitmaps)
            
        kept_bitmaps = bitmaps[first_offset:last_offset]
        
        for g in kept_glyphs:
            g[0] -= first_offset

        bitmap_str = ",\n    ".join(", ".join(f"0x{b:02X}" for b in kept_bitmaps[i:i+12]) for i in range(0, len(kept_bitmaps), 12))
        glyph_str = ",\n    ".join(f"{{ {g[0]}, {g[1]}, {g[2]}, {g[3]}, {g[4]}, {g[5]} }}" for g in kept_glyphs)
        
        y_advance_match = re.search(r'\(GFXglyph\s*\*\)\s*\w+Glyphs\s*,\s*[^,]+\s*,\s*[^,]+\s*,\s*(\d+)\s*\}', text)
        y_advance = int(y_advance_match.group(1)) if y_advance_match else 14

        optimized_content = f"""#pragma once
#include <Adafruit_GFX.h>

const uint8_t {new_font_name}Bitmaps[] PROGMEM = {{
    {bitmap_str}
}};

const GFXglyph {new_font_name}Glyphs[] PROGMEM = {{
    {glyph_str}
}};

const GFXfont {new_font_name} PROGMEM = {{
    (uint8_t *){new_font_name}Bitmaps,
    (GFXglyph *){new_font_name}Glyphs,
    0x{start_char:02X}, 0x{end_char:02X}, {y_advance}
}};
"""
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(optimized_content)
        print(f"Successfully optimized: {output_file} (Reduced to {len(kept_bitmaps)} bytes)")
    except Exception as e:
        print(f"Error processing {input_file}: {e}")

if __name__ == "__main__":
    # 10pt Menu Font: Range 32 to 126 (Includes full lowercase, uppercase, and standard punctuation)
    optimize_gfx_font("Inter_Medium10pt7b.h", "Inter_Medium_10pt.h", "Inter_Medium_10pt", 32, 126)
    
    # 22pt Number Font: Range 37 to 58 (Only maps numbers, colon, and percent sign to maximize memory savings)
    optimize_gfx_font("Inter_Medium22pt7b.h", "Inter_Medium_22pt.h", "Inter_Medium_22pt", 37, 58)