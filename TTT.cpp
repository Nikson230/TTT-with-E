#include<stdio.h>
#include<string>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<cmath>



const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int MAP_SIZE = 3;



class WTexture {
private:
  SDL_Texture* texture;
  int width;
  int height;
public:
  WTexture();
  ~WTexture();

  void free();

  bool loadFromString(TTF_Font *font, std::string str, SDL_Color textColor, SDL_Renderer* renderer);

  void render(int x, int y, SDL_Renderer* renderer);

  int getWidth(){
    return width;
  }
  int getHeight(){
    return height;
  }
};


WTexture::WTexture(){
  texture = NULL;
  width = 0;
  height = 0;
}


WTexture::~WTexture(){
  free();
}


void WTexture::free(){
  SDL_DestroyTexture(texture);
  texture = NULL;

  width = 0;
  height = 0;
}


bool WTexture::loadFromString(TTF_Font* font, std::string str, SDL_Color textColor, SDL_Renderer* renderer){
    free();

    SDL_Surface* surface = TTF_RenderText_Solid(font, str.c_str(), textColor);
    if(surface == NULL){
      printf("Failed to render textSurface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
      if(renderer == NULL){
	printf("Unable to use globalRenderer! SDL Error: %s\n", SDL_GetError());
      } else {
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if(texture == NULL){
	  printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
	} else {
	  width = surface->w;
	  height = surface->h;
	}
	SDL_FreeSurface(surface);
      }
    }

    return texture!=NULL;
}


void WTexture::render(int x, int y, SDL_Renderer* renderer){
  SDL_QueryTexture(texture, NULL, NULL, &height, &width);
  SDL_Rect renderRect = {x, y, height, width};

  SDL_RenderCopy(renderer, texture, NULL, &renderRect);
}



class Position{
private:
  int x;
  int y;
public:
  int getX(){
    return x;
  }
  
  int getY(){
    return y;
  }
  
  void setX(int value);
  void setY(int value);
};


void Position::setX(int value){
  x = value;
}

void Position::setY(int value){
  y = value;
}



class Player{
private:
  Position position;
  std::string name;
  char symbol;
  bool mark;
public:
  Player(std::string name, char symbol, int x, int y, bool mark);

  int getX(){
    return position.getX();
  }

  int getY(){
    return position.getY();
  }

  std::string getName(){
    return name;
  }

  bool getMark(){
    return mark;
  }

  void changeX(int value);
  void changeY(int value);

  void setMark(bool value);
};


Player::Player(std::string name, char symbol, int x, int y,  bool mark){
  position.setX(x);
  position.setY(x);

  this->name = name;

  this->symbol = symbol;

  this->mark = mark;
}

void Player::changeX(int value){
  position.setX(position.getX() + value);
}

void Player::changeY(int value){
  position.setY(position.getY() + value);
}

void Player::setMark(bool value){
  mark = value;
}



bool init(SDL_Window** window, SDL_Renderer** renderer);
bool loadTextures(TTF_Font *font, WTexture* texture, std::string str, SDL_Renderer* renderer);
void createMap(std::string strArr[MAP_SIZE], bool toFill, int size);
void input();
void close(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);



void createMap(std::string strArr[MAP_SIZE], int size, bool toFill){
  for(int i=0; i < size; i++){
    if(strArr[i] != ""){
      strArr[i] = "";
    }
    for(int j=0; j < size; j++){
      if(toFill){
	strArr[i] += 'O';
      } else {
	strArr[i] += ' ';
      }
    }
  }
}


void input(){
  
}


int main(int argc, char *argv[]){
  SDL_Window* window = NULL;
  SDL_Renderer* globalRenderer;

  TTF_Font* font = NULL;

  std::string testMap[] = {"",
    "",
  ""};

  createMap(testMap, MAP_SIZE, false);

  for(int i=0; i < MAP_SIZE; i++){
    printf("%s\n", testMap[i].c_str());
  }

  WTexture mapTexture;
  
  if(!init(&window, &globalRenderer)){
    printf("Error with init\n");
  } else {
      bool quit = false;
      bool toShow = true;
      
      SDL_Event event;

      //Player player("Player", 'X', 0, 0, true);

      
      while(!quit){
	while(SDL_PollEvent(&event) != 0){
	  if(event.type == SDL_QUIT){
	    quit = true;
	  }
	}
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if(keyState[SDL_SCANCODE_UP]){
	  printf("UP\n");
	} else if(keyState[SDL_SCANCODE_DOWN]){
	  printf("DOWN\n");
	} else if(keyState[SDL_SCANCODE_LEFT]){
	  printf("LEFT\n");
	} else if(keyState[SDL_SCANCODE_RIGHT]){
	  printf("RIGHT\n");
	}
	
	input();

	if(toShow){
	  SDL_RenderClear(globalRenderer);
	  for(int i=0; i < MAP_SIZE; i++){
	    if(!loadTextures(font, &mapTexture, testMap[i], globalRenderer)){
	      printf("Error with font loading\n");
	    } else {
	      SDL_SetRenderDrawColor(globalRenderer, 0x00, 0x00, 0x00, 0xFF);

	      mapTexture.render(0, i*25, globalRenderer);//const value to make multiline drawing

	      SDL_RenderPresent(globalRenderer);
	    }
	  }
	  toShow = false;
	}
      }
  }
  close(window, globalRenderer, font);
  
  return 0;
}



bool init(SDL_Window** window, SDL_Renderer** renderer){
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
    printf("SDL couldn't initialize! SDL Error %s\n", SDL_GetError());
    return false;
  } else {
    if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, window, renderer) != 0){
      printf("Unable to create window and default renderer! SDL Error: %s\n", SDL_GetError());
      return false;
    } else {
      SDL_SetRenderDrawColor(*renderer, 0x00, 0x00, 0x00, 0xFF);
      if(TTF_Init() == -1) {
	printf("SDL_ttf couldn't initialize! SDL Error: %s\n", TTF_GetError());
	return false;
      }
    }
  }
  return true;
}


bool loadTextures(TTF_Font* font, WTexture* texture, std::string str, SDL_Renderer* renderer){
  font = TTF_OpenFont("Courier Prime.ttf", 32);
  if(font == NULL){
    printf("Falied to load font! SDL_ttf Error: %s\n", TTF_GetError());
    return false;
  } else {
    SDL_Color textColor = {0xFF,0xFF,0xFF};

    if(!texture->loadFromString(font, str, textColor, renderer)){
      printf("Failed to render texture\n");
      return false;
    } 
  }
  return true;
}


void close(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font){
  SDL_DestroyRenderer(renderer);
  renderer = NULL;

  SDL_DestroyWindow(window);
  window = NULL;

  font = NULL;

  SDL_Quit();
}
