#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static char get_board_at(game_state_t *state, int x, int y);
static void set_board_at(game_state_t *state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t *state, int snum);
static char next_square(game_state_t *state, int snum);
static void update_tail(game_state_t *state, int snum);
static void update_head(game_state_t *state, int snum);

/* Helper function to get a character from the board (already implemented for you). */
static char get_board_at(game_state_t *state, int x, int y)
{
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for you). */
static void set_board_at(game_state_t *state, int x, int y, char ch)
{
  state->board[y][x] = ch;
}

/* Task 1 */
game_state_t *create_default_state()
{
  // x equals to column and y equals to row, locate chat at board[y][x]
  game_state_t *game_state = (game_state_t *)malloc(sizeof(game_state_t));
  game_state->x_size = 14;
  game_state->y_size = 10;
  game_state->board = (char **)malloc(sizeof(char *) * game_state->y_size);
  for (int i = 0; i < game_state->y_size; i++)
    game_state->board[i] = (char *)malloc(sizeof(char) * game_state->x_size);
  for (int i = 0; i < game_state->y_size; i++)
  {
    for (int j = 0; j < game_state->x_size; j++)
    {
      game_state->board[i][j] = ' ';
    }
  }
  for (int i = 0; i < game_state->x_size; i++)
  {
    game_state->board[0][i] = '#';
    game_state->board[game_state->y_size - 1][i] = '#';
  }
  for (int i = 0; i < game_state->y_size; i++)
  {
    game_state->board[i][0] = '#';
    game_state->board[i][game_state->x_size - 1] = '#';
  }
  game_state->board[2][9] = '*';
  game_state->num_snakes = 1;
  game_state->snakes = (snake_t *)malloc(sizeof(snake_t));
  game_state->snakes[0].head_x = 5, game_state->snakes[0].head_y = 4;
  game_state->snakes[0].tail_x = 4, game_state->snakes[0].tail_y = 4;
  game_state->snakes[0].live = true;
  game_state->board[4][4] = 'd';
  game_state->board[4][5] = '>';
  return game_state;
}

/* Task 2 */
void free_state(game_state_t *state)
{
  // TODO: Implement this function.
  free(state->snakes);
  for (int i = 0; i < state->y_size; i++)
  {
    free(state->board[i]);
  }
  free(state->board);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp)
{
  // TODO: Implement this function.
  for (int i = 0; i < state->y_size; i++)
  {
    for (int j = 0; j < state->x_size; j++)
    {
      fprintf(fp, "%c", get_board_at(state, j, i));
    }
    fprintf(fp, "\n");
  }
  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t *state, char *filename)
{
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c)
{
  // TODO: Implement this function.
  if (c == 'w' || c == 'a' || c == 's' || c == 'd')
    return true;
  return false;
}

static bool is_snake(char c)
{
  // TODO: Implement this function.
  if (c == 'w' || c == 'a' || c == 's' || c == 'd' || c == '>' || c == '<' || c == '^' || c == 'v' || c == 'x')
    return true;
  return false;
}

static char body_to_tail(char c)
{
  // TODO: Implement this function.
  switch (c)
  {
  case '>':
    return 'd';
  case '<':
    return 'a';
  case '^':
    return 'w';
  case 'v':
    return 's';
  default:
    return ' ';
  }
}

static int incr_x(char c)
{
  // TODO: Implement this function.
  if (c == '>' || c == 'd')
  {
    return 1;
  }
  if (c == '<' || c == 'a')
  {
    return -1;
  }
  return 0;
}

static int incr_y(char c)
{
  // TODO: Implement this function.
  if (c == '^' || c == 'w')
  {
    return -1;
  }
  if (c == 'v' || c == 's')
  {
    return 1;
  }
  return 0;
}

/* Task 4.2 */
static char next_square(game_state_t *state, int snum)
{
  // TODO: Implement this function.
  char c = get_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y);
  printf("..%c..\n",c);
  return get_board_at(state, state->snakes[snum].head_x + incr_x(c), state->snakes[snum].head_y + incr_y(c));
}

/* Task 4.3 */
static void update_head(game_state_t *state, int snum)
{
  // TODO: Implement this function.
  if (!state->snakes[snum].live)
    return;
  char c = get_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y);
  set_board_at(state, state->snakes[snum].head_x + incr_x(c), state->snakes[snum].head_y + incr_y(c), c);
  state->snakes[snum].head_x = state->snakes[snum].head_x + incr_x(c);
  state->snakes[snum].head_y = state->snakes[snum].head_y + incr_y(c);
  return;
}

/* Task 4.4 */
static void update_tail(game_state_t *state, int snum)
{
  // TODO: Implement this function.
  char c = get_board_at(state, state->snakes[snum].tail_x, state->snakes[snum].tail_y);
  set_board_at(state, state->snakes[snum].tail_x, state->snakes[snum].tail_y, ' ');
  state->snakes[snum].tail_x = state->snakes[snum].tail_x + incr_x(c);
  state->snakes[snum].tail_y = state->snakes[snum].tail_y + incr_y(c);
  set_board_at(state, state->snakes[snum].tail_x, state->snakes[snum].tail_y,
               body_to_tail(get_board_at(state, state->snakes[snum].tail_x, state->snakes[snum].tail_y)));
  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state))
{
  // TODO: Implement this function.
  for (int i = 0; i < state->num_snakes; i++)
  {
    if(!state->snakes[i].live){
      continue;
    }
    char c = next_square(state, i);
    if (c == '*')
    {
      update_head(state, i);
      add_food(state);
    }
    else if (c == '#' || is_snake(c))
    {
      set_board_at(state, state->snakes[i].head_x, state->snakes[i].head_y, 'x');
      state->snakes[i].live = false;
    }
    else
    {
      update_head(state, i);
      update_tail(state, i);
    }
  }
  return;
}

/* Task 5 */
game_state_t *load_board(char *filename)
{
  // TODO: Implement this function.
  FILE *fp = fopen(filename, "r");
  char buffer[MAX_SIZE][MAX_SIZE];
  int i;
  for (i = 0; fgets(buffer[i], MAX_SIZE, fp); i++)
  {
  };
  game_state_t *state = (game_state_t *)malloc(sizeof(game_state_t));
  state->x_size = strlen(buffer[0]) - 1;
  state->y_size = i;
  state->board = (char **)malloc(sizeof(char *) * state->y_size);
  for (int i = 0; i < state->y_size; i++)
    state->board[i] = (char *)malloc(sizeof(char) * state->x_size);
  for (int i = 0; i < state->y_size; i++)
  {
    for (int j = 0; j < state->x_size; j++)
    {
      state->board[i][j] = buffer[i][j];
    }
  }
  return state;
}

/* Task 6.1 */
static void find_head(game_state_t *state, int snum)
{
  // TODO: Implement this function.
  int cnt = snum;
  for (int i = 0; i < state->y_size; i++)
  {
    for (int j = 0; j < state->x_size; j++)
    {
      char c = get_board_at(state, j, i);
      if (c == 'x' || (is_snake(c) && !is_snake(get_board_at(state, j + incr_x(c), i + incr_y(c)))))
      {
        if (cnt)
          cnt--;
        else
        {
          state->snakes[snum].head_x = j;
          state->snakes[snum].head_y = i;
          return;
        }
      }
    }
  }
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state)
{
  // TODO: Implement this function.
  int num = 0, tail[MAX_SIZE][2];
  for (int i = 0; i < state->y_size; i++)
  {
    for (int j = 0; j < state->x_size; j++)
    {
      if (is_tail(get_board_at(state, j, i)))
      {
        tail[num][0] = i, tail[num][1] = j;
        num++;
      }
    }
  }
  state->num_snakes = num;
  state->snakes=(snake_t*)malloc(sizeof(snake_t)*num);
  for (int i = 0; i < num; i++)
  {
    find_head(state, i);
    state->snakes[i].tail_x = tail[i][1];
    state->snakes[i].tail_y = tail[i][0];
    state->snakes[i].live = !(get_board_at(state, state->snakes[i].head_x, state->snakes[i].head_y) == 'x');
  }
  return state;
}
