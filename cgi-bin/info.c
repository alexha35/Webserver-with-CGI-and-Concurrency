// names: Alex Ha, Kennaa Yared
// x500: haxxx185, yared006

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>


void header (char* mime) {
  printf("HTTP/1.1 200 OK\n");
  printf("Content_Type:%s\n\n",mime);
}

void begin(char* title) {
  printf("<html><head><title>%s</title></head><body>", title);
}

void finish(void) {
  printf("</body></html>");
}

int main(int argc, char const *args[]) {
  char* query;

  header("text/html");
  begin("Game Preference");

  char gamePref[15];
  strcpy(gamePref,args[0]);
  if (gamePref) {
    printf("<p>Your battle royal game of choice is: %s</p>", gamePref);
  }
  else {
    printf("<p>error: no query string</p>");
  }

  if (!strcmp(gamePref, "fortnite") || !strcmp(gamePref, "Apex legends")) {
    printf("<h2>This is the battle royal game you chose</h2>");
    printf("<img src=\"../images/fortniteVSapex.jpeg\">");
  }
  else {
    printf("<h2>You entered a game...</h2>");
    printf("<p>Nothing is going to happen...</p>");
  }

  finish();

  return 0;
}
