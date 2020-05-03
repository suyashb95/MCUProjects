#include <stdint.h>
#include <LCD.h>
#include <SPI.h>

#define m 32
#define n 31

char main_grid[m][n];
char temp_grid[m][n];

void setup() {
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.begin();

    Tft.lcd_init();
    Tft.lcd_clear_screen(BLACK);
    init_main_grid();
    init_temp_grid();
    set_r_pentonimo();
    render_grid();
}

void loop() {
  calculate_next_generation();
  render_grid_fast();
}

void calculate_next_generation() {
    for(uint8_t i=0; i<m; i++) {
        for(uint8_t j=0; j<n; j++) {
            temp_grid[i][j] = should_stay_alive_or_resurrect(i, j) ? '1': '0';
        }
    }    
}

void render_grid() {
    for (uint8_t i=0; i<m; i++)
        for (uint8_t j=0; j<n; j++) {
            main_grid[i][j] = temp_grid[i][j];
            if(is_alive(i, j))
                Tft.lcd_fill_rect(i*4 + 75, j*4+ 75, 4, 4, RED);
            else
                Tft.lcd_fill_rect(i*4 + 75, j*4+ 75, 4, 4, BLACK);
        }
}

void render_grid_fast() {
    for(uint8_t i=0; i<m; i++) {
        for(uint8_t j=0; j<n; j++) {
          boolean was_alive = is_alive(i, j);
          main_grid[i][j] = temp_grid[i][j];
          boolean resurrected = is_alive(i, j);
          if(resurrected && !was_alive)
            Tft.lcd_fill_rect_fast(i*4 + 75, j*4 + 75, 4, 4, RED);
          if (!resurrected && was_alive)
            Tft.lcd_fill_rect_fast(i*4 + 75, j*4 + 75, 4, 4, BLACK);
        }
    }
}

void init_main_grid() {
    for (uint8_t i=0; i<m; i++)
        for (uint8_t j=0; j<n; j++)
            main_grid[i][j] = '0';
}

void init_temp_grid() {
    for (uint8_t i=0; i<m; i++)
        for (uint8_t j=0; j<n; j++)
            temp_grid[i][j] = '0';
}

void set_r_pentonimo() {
    temp_grid[m/2][n/2] = '1';
    temp_grid[m/2][n/2+1] = '1';
    temp_grid[m/2][n/2-1] = '1';
    temp_grid[m/2-1][n/2] = '1';
    temp_grid[m/2+1][n/2+1] = '1';
}

boolean should_stay_alive_or_resurrect(uint8_t i, uint8_t j) {
    uint8_t live_neighbors = get_live_neighbors(i, j);
    return (is_alive(i, j) && (live_neighbors == 2 || live_neighbors == 3)) || (live_neighbors == 3);
}

int get_live_neighbors(uint8_t i, uint8_t j) {
    uint8_t live_neighbors = 0;
    if (is_alive(i-1, j)) live_neighbors += 1;
    if (is_alive(i+1, j)) live_neighbors += 1;
    if (is_alive(i, j-1)) live_neighbors += 1;
    if (is_alive(i, j+1)) live_neighbors += 1;
    if (is_alive(i-1, j-1)) live_neighbors += 1;
    if (is_alive(i-1, j+1)) live_neighbors += 1;
    if (is_alive(i+1, j-1)) live_neighbors += 1;
    if (is_alive(i+1, j+1)) live_neighbors += 1;
    return live_neighbors;
}

boolean is_valid_index(uint8_t i, uint8_t j) {
    return ((i>0) && (i<m) && (j>0) && (j<n));
}

boolean is_alive(uint8_t i, uint8_t j) {
  return is_valid_index(i, j) && main_grid[i][j] == '1';
}
