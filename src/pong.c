#include <pebble.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

static Window *s_main_window;
static Window *s_side_window;
static Layer *s_pad_layer;
static TextLayer *s_scr;
static Layer *s_pad2_layer;
static TextLayer *s_scoreb;
static TextLayer *s_score1;
static TextLayer *s_score2;
static TextLayer *s_score3;
static TextLayer *s_score4;
static TextLayer *s_score5;
static int score1 = 5;
static int score2 = 4;
static int score3 = 3;
static int score4 = 2;
static int score5 = 1;
static int pos;
static int spd;
static GPoint bpos; 
static GPoint bspd;
static int score;
static int c = 0;
static int pausex;
static int pausey;


static void pad_update_proc(Layer *this_layer, GContext *ctx) 
  {
  //Background
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(s_pad_layer), 0, GCornerNone);
	pos += (spd*.7);
  //Paddle bounds
	if(pos <= 0)
    {
		pos = 0;
	  }
	else if(pos >= 105)
    {
		pos = 105;
	  }
  //Paddle
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, GRect(10, pos , 7, 45), 0, GCornerNone);
	//Ball bounce off right
	if(bpos.x >= 130)
    {
		bspd.x = bspd.x * -1;
		score += 1;
		//Score
	  static char sco[32];
	  snprintf(sco, sizeof(sco), "Score %i", score);
    text_layer_set_text(s_scr, sco);
	  }
  //Ball bounce off top or bot
	if(bpos.y <= 5 || bpos.y >= 140)
    {
		bspd.y = bspd.y * -1;
	  }
  //Change in speed
	if(bpos.x >= 16 && bpos.x < 20)
    {
		if(abs(bpos.y + 5 - (pos+23)) < 23)
      {
			if(abs(bpos.y - pos - 11) <12)
        {
				if(bspd.y < 10)
          {
          bspd.y += 1;
          }
				else
          {
          bspd.y = 10;
          }
			  }
			else if(abs(bpos.y - pos + 11) < 12)
        {
				if(bspd.y > -10)
          {
          bspd.y -= 1;
          }
				else
          {
          bspd.y = -10;
          }
			  }
			bspd.x *= -1;
		  }
			if(bpos.x <= 5)
      {
			bspd.x = bspd.x * -1;
			bpos.x += bspd.x;
			
			layer_mark_dirty(s_pad_layer);
			score -= 1;
			
	    static char sco[32];
	    snprintf(sco, sizeof(sco), "Score %i", score);
      text_layer_set_text(s_scr, sco);
		  }
	  }
/*  if(bpos.x < 5)      //Suppose to vibrate
    {
    vibes_short_pulse();
    }*/
  //Change in HighScore
	bpos.x += bspd.x;
	bpos.y += bspd.y;
  //The ball
	graphics_fill_rect(ctx, GRect(bpos.x, bpos.y, 10, 10), 0, GCornerNone);
}

static void side_window_load(Window *window)
  {
  Layer *window_layer =  window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
  s_scoreb = text_layer_create(GRect(20, 0, 144, 50));
  text_layer_set_text_color(s_scoreb, GColorBlack);
  text_layer_set_font(s_scoreb, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  static char sco[12];
  snprintf(sco, sizeof(sco), "High Score");
  text_layer_set_text(s_scoreb, sco);
  
  
  s_score1 = text_layer_create(GRect(25, 35, 144, 50));
  text_layer_set_text_color(s_score1, GColorBlack);
  text_layer_set_font(s_score1, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  static char sco1[15];
  snprintf(sco1, sizeof(sco1), "1st          %i", score1);
  text_layer_set_text(s_score1, sco1);
  
  s_score2 = text_layer_create(GRect(25, 55, 144, 50));
  text_layer_set_text_color(s_score2, GColorBlack);
  text_layer_set_font(s_score2, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  static char sco2[15];
  snprintf(sco2, sizeof(sco2), "2nd         %i", score2);
  text_layer_set_text(s_score2, sco2);
  
  s_score3 = text_layer_create(GRect(25, 75, 144, 55));
  text_layer_set_text_color(s_score3, GColorBlack);
  text_layer_set_font(s_score3, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  static char sco3[15];
  snprintf(sco3, sizeof(sco3), "3rd          %i", score3);
  text_layer_set_text(s_score3, sco3);
  
  s_score4 = text_layer_create(GRect(25, 95, 144, 55));
  text_layer_set_text_color(s_score4, GColorBlack);
  text_layer_set_font(s_score4, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  static char sco4[15];
  snprintf(sco4, sizeof(sco4), "4th          %i", score4);
  text_layer_set_text(s_score4, sco4);
  
  s_score5 = text_layer_create(GRect(25, 115, 144, 55));
  text_layer_set_text_color(s_score5, GColorBlack);
  text_layer_set_font(s_score5, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  static char sco5[15];
  snprintf(sco5, sizeof(sco5), "5th          %i", score5);
  text_layer_set_text(s_score5, sco5);
  
  s_pad2_layer = layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  layer_add_child(window_layer, s_pad2_layer);
  layer_add_child(window_layer, text_layer_get_layer(s_scoreb));
  layer_add_child(window_layer, text_layer_get_layer(s_score1));
  layer_add_child(window_layer, text_layer_get_layer(s_score2));
  layer_add_child(window_layer, text_layer_get_layer(s_score3));
  layer_add_child(window_layer, text_layer_get_layer(s_score4));
  layer_add_child(window_layer, text_layer_get_layer(s_score5));
  }

static void side_window_unload(Window *window)
  {
  layer_destroy(s_pad2_layer);
  text_layer_destroy(s_scoreb);
  }

static void side_init(void)
  {
  s_side_window = window_create();
  window_set_window_handlers(s_side_window, (WindowHandlers) 
    {
    .load = side_window_load,
    .unload = side_window_unload
    });
  window_stack_push(s_side_window, true);
  }

static void deinit(void) 
  {
  window_destroy(s_main_window);
	accel_data_service_unsubscribe();
	}

static void data_handler(AccelData *data, uint32_t num_samples) 
  {
	if(data[0].y < -75)
    {
    spd = 5;
    }
	else if(data[0].y > 75)
    {
    spd = -5;
    }
	else
    {
    spd = 0;
    }
	layer_mark_dirty(s_pad_layer);
  }

static void up_click_handler(ClickRecognizerRef recognizer, void *context)
  {
  layer_set_hidden(s_pad_layer, true);
  side_init();
  layer_set_hidden(s_pad_layer, false);
  }
static void select_click_handler(ClickRecognizerRef recognizer, void *context)
  {
  if(score > score1)
    {
    score5 = score4;
    score4 = score3;
    score3 = score2;
    score2 = score1;
    score1 = score;
    }
  else if (score > score2)
    {
    score5 = score4;
    score4 = score3;
    score3 = score2;
    score2 = score;
    }
  else if (score > score3)
    {
    score5 = score4;
    score4 = score3;
    score3 = score;
    }
  else if (score > score4)
    {
    score5 = score4;
    score4 = score;
    }
  else if(score > score5)
    {
    score5 = score;
    }
  pos = 50;
	spd = 0;
	bpos = GPoint(129, 65);
	bspd.y = ((rand() % 1) * 2) + 1;
	bspd.x = ((rand() % 2) * 2) + 1;
	score = -1;
  static char sco[32];
	snprintf(sco, sizeof(sco), "Score %i", score);
  text_layer_set_text(s_scr, sco);
  
  }
static void down_click_handler(ClickRecognizerRef recognizer, void *context)
  {
  c++;
  if (c % 2 == 1)
    {
    pausex = bspd.x;
    bspd.x = 0;
    pausey = bspd.y;
    bspd.y = 0;
    accel_data_service_unsubscribe();
    }
  else
    {
    bspd.x = pausex;
    bspd.y = pausey;
    int num_samples = 3;
    accel_data_service_subscribe(num_samples, data_handler);
	  accel_service_set_sampling_rate(ACCEL_SAMPLING_100HZ);
    }
  }

static void click_config_provider(void *context)
  {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  }

static void main_window_load(Window *window) 
  {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
	s_scr = text_layer_create(GRect(50, 0, 144, 50));
  text_layer_set_background_color(s_scr, GColorClear);
  text_layer_set_text_color(s_scr, GColorWhite);
	static char sco[32];
	snprintf(sco, sizeof(sco), "Score %i", score);
  text_layer_set_text(s_scr, sco);
  s_pad_layer = layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  layer_add_child(window_layer, s_pad_layer);
	layer_add_child(window_layer, text_layer_get_layer(s_scr));
  layer_set_update_proc(s_pad_layer, pad_update_proc);
  }

static void main_window_unload(Window *window) 
  {
  layer_destroy(s_pad_layer);
	text_layer_destroy(s_scr);
  }

static void init(void) 
  {
  s_main_window = window_create();
  window_set_click_config_provider(s_main_window, click_config_provider);
  window_set_window_handlers(s_main_window, (WindowHandlers) 
    {
    .load = main_window_load,
    .unload = main_window_unload
    });
  window_stack_push(s_main_window, true);
	int num_samples = 3;
  accel_data_service_subscribe(num_samples, data_handler);
	accel_service_set_sampling_rate(ACCEL_SAMPLING_100HZ);
	pos = 50;
	spd = 0;
	bpos = GPoint(129, 65);
	bspd.y = (rand() % 1)*((2 * rand() % 2) + 1);
	bspd.x = (rand() % 2)*((2 * rand() % 2) + 1);
	score = -1;
	}

int main(void) 
  {
  init();
  app_event_loop();
  deinit();
  }
