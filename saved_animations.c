void Traffic_lights() {
    while (1)
    {
        // 🔴 RED
        ws2812_set_pixel(2, 255, 0, 0);   // Red ON
        ws2812_set_pixel(1, 0, 0, 0);
        ws2812_set_pixel(0, 0, 0, 0);
        ws2812_show();
        _delay_ms(3000);

        // 🔴🟡 RED + YELLOW
        ws2812_set_pixel(2, 255, 0, 0);
        ws2812_set_pixel(1, 255, 150, 0);
        ws2812_set_pixel(0, 0, 0, 0);
        ws2812_show();
        _delay_ms(1000);

        // 🟢 GREEN
        ws2812_set_pixel(2, 0, 0, 0);
        ws2812_set_pixel(1, 0, 0, 0);
        ws2812_set_pixel(0, 0, 255, 0);
        ws2812_show();
        _delay_ms(3000);

        // 🟡 YELLOW
        ws2812_set_pixel(2, 0, 0, 0);
        ws2812_set_pixel(1, 255, 150, 0);
        ws2812_set_pixel(0, 0, 0, 0);
        ws2812_show();
        _delay_ms(1000);
    }
}

void dot_send() {
    while (1)
    {
        uint8_t p = 0;
        for(uint8_t i = 0; i < LED_COUNT; i++)
        {
            if(i == 0) { p = 80; }
            else { p = i - 1; }
            ws2812_set_pixel(i, 0, 255, 255);
            ws2812_set_pixel(p, 0, 0, 0);
            ws2812_show();
            _delay_ms(30);
        }
    }
}

void starter_code() {
    uint8_t packet_len = 3;
    uint8_t pointer[] = {0, 14, 28};

    for(int i = 0; i < LED_COUNT; i++)
    {
        for(int j = 0; j < 3; j++){
            if(i - pointer[j] >= 0 && i - pointer[j] < LED_COUNT) {
                ws2812_set_pixel(i - pointer[j], 130, 90, 40);
            }
            if(i - pointer[j] - packet_len >= 0 && i - pointer[j] - packet_len < LED_COUNT) {
                ws2812_set_pixel(i - pointer[j] - packet_len, 0, 0, 0);
            }
        }
        ws2812_show();
        _delay_ms(25);
    }
    uint8_t idx;
    for(int k = 0; k < 100; k++){
        for(int i = 81; i < 95; i++){
            for(int j = 0; j < 3; j++){
                idx = i - pointer[j];
                if (idx >= LED_COUNT) {
                    idx -= 42;
                }
                if(idx >= 40 && idx < LED_COUNT) {
                    ws2812_set_pixel(idx, 130, 90, 40);
                }
                idx = i - pointer[j] - packet_len;
                if (idx >= LED_COUNT) {
                    idx -= 42;
                }
                if(idx >= 0 && idx < LED_COUNT) {
                    ws2812_set_pixel(idx, 0, 0, 0);
                }
            }
            ws2812_show();
            _delay_ms(25);
        }
    }
}

void ir_echo() {
    ws2812_init();
    IR_init();

    uint8_t pointer_count = 40; // Number of moving dots
    uint8_t pointers[pointer_count];

    for (uint8_t i = 0; i < pointer_count; i++)
    {
        pointers[i] = LED_COUNT - 1;
    }

    while (1)
    {
        // Clear entire strip first
        for(uint8_t i = 0; i < LED_COUNT; i++)
        {
            ws2812_set_pixel(i, 0, 0, 0);
        }

        // Move pointers
        for(uint8_t i = 0; i < pointer_count; i++)
        {
            if(pointers[i] >= LED_COUNT)
            {
                continue;  // skip if out of range
            }

            ws2812_set_pixel(pointers[i], 255, 150, 0);

            pointers[i]++;

            if(pointers[i] >= LED_COUNT)
            {
                pointers[i] = LED_COUNT; // mark as finished
            }
        }

        ws2812_show();


        if(IR_detect())
        {
            for(uint8_t i = 0; i < pointer_count; i++)
            {
                if(pointers[i] >= LED_COUNT)
                {
                    pointers[i] = 0;  // restart animation
                    break;
                }
            }
        }

        _delay_ms(30);
    }
}

int ir_sensor_input_stip(void)
{
    ws2812_init();
    IR_init();

    uint8_t pointer_count = 40; // Number of moving dots
    uint8_t pointers[pointer_count];

    for (uint8_t i = 0; i < pointer_count; i++)
    {
        pointers[i] = LED_COUNT - 1;
    }

    while (1)
    {
        // Clear entire strip first
        for(uint8_t i = 0; i < LED_COUNT; i++)
        {
            ws2812_set_pixel(i, 0, 0, 0);
        }

        // Move pointers
        for(uint8_t i = 0; i < pointer_count; i++)
        {
            if(pointers[i] >= LED_COUNT)
            {
                continue;  // skip if out of range
            }

            ws2812_set_pixel(pointers[i], 255, 150, 0);

            pointers[i]++;

            if(pointers[i] >= LED_COUNT)
            {
                pointers[i] = LED_COUNT; // mark as finished
            }
        }

        ws2812_show();


        if(IR_detect())
        {
            for(uint8_t i = 0; i < pointer_count; i++)
            {
                if(pointers[i] >= LED_COUNT)
                {
                    pointers[i] = 0;  // restart animation
                    break;
                }
            }
        }

        _delay_ms(30);
    }
}

void hsv_to_rgb(uint16_t h, uint8_t s, uint8_t v,
                uint8_t *r, uint8_t *g, uint8_t *b)
{
    uint8_t region = h / 43;
    uint16_t remainder = (h - (region * 43)) * 6;

    uint8_t p = (v * (255 - s)) >> 8;
    uint8_t q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    uint8_t t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0: *r = v; *g = t; *b = p; break;
        case 1: *r = q; *g = v; *b = p; break;
        case 2: *r = p; *g = v; *b = t; break;
        case 3: *r = p; *g = q; *b = v; break;
        case 4: *r = t; *g = p; *b = v; break;
        default: *r = v; *g = p; *b = q; break;
    }
}

void hsv() {
    ws2812_init();
    IR_init();

    uint16_t base_hue = 0;

while (1)
{
    for (uint8_t i = 0; i < 40; i++)
    {
        uint16_t hue = base_hue + (i * 256 / 40);  // spread hue across strip
        uint8_t r, g, b;

        hsv_to_rgb(hue % 256, 255, 255, &r, &g, &b);

        ws2812_set_pixel(i, r, g, b);
    }

    ws2812_show();

    base_hue += 2;  // speed of movement
    _delay_ms(20);
}
}


static void render_anim_packets(void)
{
    uint8_t pointer[3] = {0, 14, 28};

    if(anim_tick == 0){
        ws2812_clear();
    }

    

    if(count < 15){
        uint8_t idx = anim_tick / 2.5;
        if(idx < 81){
            for (uint8_t j = 0; j < 3; j++)
            {
                for(int j = 0; j < 3; j++){
                    if(idx - pointer[j] >= 0 && idx - pointer[j] < LED_COUNT) {
                        ws2812_set_pixel(idx - pointer[j], 130, 90, 40);
                    }
                    if(idx - pointer[j] - PACKET_LEN >= 0 && idx - pointer[j] - PACKET_LEN < LED_COUNT) {
                        ws2812_set_pixel(idx - pointer[j] - PACKET_LEN, 0, 0, 0);
                    }
                }
            }
        }else{
            for(int j = 0; j < 3; j++){
                uint16_t pv_idx = idx - pointer[j];
                if (pv_idx >= LED_COUNT) {
                    pv_idx -= 42;
                }
                if(pv_idx >= 40 && pv_idx < LED_COUNT) {
                    ws2812_set_pixel(pv_idx, 130, 90, 40);
                }
                pv_idx = idx - pointer[j] - PACKET_LEN;
                if (pv_idx >= LED_COUNT) {
                    pv_idx -= 42;
                }
                if(pv_idx >= 0 && pv_idx < LED_COUNT) {
                    ws2812_set_pixel(pv_idx, 0, 0, 0);
                }
            }
        }
        if (anim_tick >= PACKET_TICK_LEN) {
            anim_tick = 203;
            count++;
        }
    }else if(count < 17) {
        if(count == 15){
            anim_tick = 0;
            ws2812_clear();
            count++;
        }
        uint8_t idx = anim_tick / 2.5;

        for (uint8_t j = 0; j < 3; j++)
        {
            for(int j = 0; j < 3; j++){
                uint8_t start_idx = LED_COUNT - idx - pointer[j];
                if(start_idx >= 0 && start_idx < LED_COUNT) {
                    ws2812_set_pixel(start_idx, 130, 90, 40);
                }
                uint8_t end_idx = LED_COUNT - idx - pointer[j] + PACKET_LEN;
                if(end_idx >= 0 && end_idx < LED_COUNT) {
                    ws2812_set_pixel(end_idx, 0, 0, 0);
                }
            }
        }
        if (anim_tick >= PACKET_TICK_LEN) {
            anim_tick = 0;
            count++;
        }
    }else {
        uint8_t idx = anim_tick / 2.5;
        if(idx > 40) return;
        
        ws2812_set_pixel(idx, 130, 90, 40);
    }

    

    anim_tick++;
    
}

static void render_anim_packets(void)
{
    uint8_t pointer[3] = {0, 14, 28};

    if(anim_tick == 0){
        ws2812_clear();
    }

    uint8_t idx = anim_tick / 2.5;

    if(count > 15){  
        
    }

    if(count < 15){
        if(idx < 81){
        for (uint8_t j = 0; j < 3; j++)
        {
            for(int j = 0; j < 3; j++){
                if(idx - pointer[j] >= 0 && idx - pointer[j] < LED_COUNT) {
                    ws2812_set_pixel(idx - pointer[j], 130, 90, 40);
                }
                if(idx - pointer[j] - PACKET_LEN >= 0 && idx - pointer[j] - PACKET_LEN < LED_COUNT) {
                    ws2812_set_pixel(idx - pointer[j] - PACKET_LEN, 0, 0, 0);
                }
            }
        }
        }else{
            for(int j = 0; j < 3; j++){
                uint16_t pv_idx = idx - pointer[j];
                if (pv_idx >= LED_COUNT) {
                    pv_idx -= 42;
                }
                if(pv_idx >= 40 && pv_idx < LED_COUNT) {
                    ws2812_set_pixel(pv_idx, 130, 90, 40);
                }
                pv_idx = idx - pointer[j] - PACKET_LEN;
                if (pv_idx >= LED_COUNT) {
                    pv_idx -= 42;
                }
                if(pv_idx >= 0 && pv_idx < LED_COUNT) {
                    ws2812_set_pixel(pv_idx, 0, 0, 0);
                }
            }
        }
        if (anim_tick >= PACKET_TICK_LEN){
            anim_tick = 203;
            count++;
        }
    }else{
        if(count == 15){
            ws2812_clear();
            count++;
        }
        if(led_bar_idx <= 81){
            ws2812_set_pixel(led_bar_idx, 130, 90, 40);
            led_bar_idx++;
        }else{
            return;
        }
    }
    anim_tick++;
    
}