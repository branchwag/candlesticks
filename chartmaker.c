#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_CANDLES 1000
#define CHART_MARGIN 50

typedef struct {
  float open;
  float high;
  float low;
  float close;
} Candle;

int parse_candles(const char *filename, Candle candles[], int max_candles) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Error opening file");
    return 0;
  }

  float previous_close = 0.0f;
  int count = 0;
  srand(time(NULL));

  while (count < max_candles && fscanf(file, "%f\n", &candles[count].close) == 1) {
    if (count == 0) {
      candles[count].open = candles[count].close;
    } else {
      candles[count].open = previous_close;
    }

    float variation = (rand() % 10) / 10.0f;
    candles[count].high = candles[count].close + variation;
    candles[count].low = candles[count].close - variation;

    previous_close = candles[count].close;
    count ++;
  } 

  fclose(file);
  return count;
}

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Candlestick Chart");
  SetTargetFPS(60);

  Candle candles[MAX_CANDLES];
  int num_candles = parse_candles("feed.log", candles, MAX_CANDLES);

  int chart_x = CHART_MARGIN;
  int chart_y = CHART_MARGIN;
  int chart_width = SCREEN_WIDTH - 2 * CHART_MARGIN;
  int chart_height = SCREEN_HEIGHT - 2 * CHART_MARGIN;
  int candle_width = chart_width / num_candles;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

   for (int i = 0; i < num_candles; i++) {
	    float open_y = chart_y + chart_height - (candles[i].open / 110.0f) * chart_height;
            float close_y = chart_y + chart_height - (candles[i].close / 110.0f) * chart_height;
            float high_y = chart_y + chart_height - (candles[i].high / 110.0f) * chart_height;
            float low_y = chart_y + chart_height - (candles[i].low / 110.0f) * chart_height;

            int x = chart_x + i * candle_width;

	    DrawLine(x + candle_width / 2, high_y, x + candle_width / 2, low_y, BLACK);

	    if (candles[i].open > candles[i].close) {
                // Bearish candle (open > close)
                DrawRectangle(x, close_y, candle_width, open_y - close_y, RED);
            } else {
                // Bullish candle (open < close)
                DrawRectangle(x, open_y, candle_width, close_y - open_y, GREEN);
            }
        } 

	DrawText("Candlestick Chart", 10, 10, 20, BLACK);
	EndDrawing();
  }
  
  CloseWindow();
  return 0;
}
