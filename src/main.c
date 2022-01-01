#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include "./constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer *renderer = NULL;

int	last_frame_time = 0;
int	x_direction = 0;
int	y_direction = 0;

struct ball {
  float x;
  float y;
  float width;
  float height;
}ball;

struct paddle {
  float x;
  float y;
  float width;
  float height;
}paddle;

int	initialize_window(void)
{
 if (SDL_Init(SDL_INIT_EVERYTHING != 0))
 {
   fprintf(stderr, "Error initializing SDL. \n");
 	return (FALSE);
 }
 window = SDL_CreateWindow(
     NULL,
     SDL_WINDOWPOS_CENTERED,
     SDL_WINDOWPOS_CENTERED,
     WINDOW_WIDTH,
     WINDOW_HEIGHT,
     SDL_WINDOW_BORDERLESS 
 );
 if (!window)
 {
   fprintf(stderr, "Error creating SDL Window. \n");
   return (FALSE);
 }
 renderer = SDL_CreateRenderer(window, -1, 0);
 if (!renderer)
 {
   fprintf(stderr, "Error creating SDL Renderer. \n");
   return (FALSE);
 }

 return (TRUE);
}

void	process_input()
{
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type)
  {
	case SDL_QUIT:
	  game_is_running = FALSE;
	  break ;
	case SDL_KEYDOWN:
	  if (event.key.keysym.sym == SDLK_ESCAPE)
	    game_is_running = FALSE;
	  else if (event.key.keysym.sym == SDLK_LEFT && paddle.x > 0)
	      paddle.x -= 10;
	  else if (event.key.keysym.sym == SDLK_RIGHT && paddle.x < 720) 
	  	paddle.x += 10;
	  break ;
  }
}

int	random_number(int max_number)
{
  srand(time(0));
  return (rand() % max_number);
}

void	setup()
{
  	x_direction = 160;
	y_direction = 80;

	ball.x = random_number(800);
	ball.y = random_number(30);
	ball.width = BALL_WIDTH;
	ball.height = BALL_HEIGHT;

	paddle.x = 400;
	paddle.y = 560;
	paddle.width = PADDLE_WIDTH;
        paddle.height = PADDLE_HEIGHT;	
}

int	is_collision()
{
  	if (ball.x < 7.5)
	{
	  x_direction *= -1;
	  return (1);
	}
	else if (ball.x > 792.5)
	{
	  x_direction *= -1;
	  return (1);
	}
	else if (ball.y < 7.5)
	{
	  y_direction *= -1;
	  return (1);
	}
	else if (ball.y > paddle.y - 7.5 && ball.x >= paddle.x - 80 && ball.x <= paddle.x + 80)
	{
	  if (x_direction < 0)
	  {
	  	y_direction *= -1;
	  	x_direction *= 1;
	  }
	  else if (x_direction > 0)
	  {
	    y_direction *= -1;
	    x_direction *= 1;
	  }
	  return (1);
	}
	else if (ball.y > 600)
	{
	  game_is_running = FALSE;
	  return (0);
	}
	else
	  return (1);
	return (0);
}

void	update()
{
  // Waste time / sleep untill we reach the frame target
	int	time_to_wait = FRAME_TARGET_TIME - SDL_GetTicks() - last_frame_time;

	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
	{
	  SDL_Delay(time_to_wait);
	}
	// Get delta time factor converted to seconds to be used to update my objects
	float	delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
	last_frame_time = SDL_GetTicks();
	if (is_collision())	
	{
  		ball.x += x_direction * delta_time; 
		ball.y += y_direction * delta_time;
	}
}

void	render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_Rect ball_rect = { 
	  (int)ball.x, 
	  (int)ball.y, 
	  (int)ball.width, 
	  (int)ball.height 
	};

	SDL_Rect paddle_rect = { 
	  (int)paddle.x, 
	  (int)paddle.y, 
	  (int)paddle.width, 
	  (int)paddle.height 
	};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &ball_rect);
	SDL_RenderFillRect(renderer, &paddle_rect);

	SDL_RenderPresent(renderer);	
}

void	destroy_window()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int	main(void)
{
  game_is_running = initialize_window();
  
  setup();

  while (game_is_running)
  {
    process_input();
    update();
    render();
  }

  destroy_window();

  return (0);
}
