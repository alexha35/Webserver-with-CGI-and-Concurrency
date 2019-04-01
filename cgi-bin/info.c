// names: Alex Ha, Kennaa Yared
// x500: haxxx185, yared006

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>


void header (char* type) {
  printf("HTTP/1.1 200 OK\n");
  printf("Content_Type:%s\n\n",type);
}

void begin(char* title) {
  printf("<html><head><title>%s</title></head><body>", title);
}

void finish() {
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

  if (!strcmp(gamePref, "Fortnite") || !strcmp(gamePref, "Apexlegends" ||)
      !strcmp(gamePref, "fortnite") || !strcmp(gamePref, "apexlegends")) {
    printf("<h2>This is the battle royal game you chose</h2>");
    printf("<img src=\"../images/5.jpg\">");
  }
  else {
    printf("<h2>Unknown entry</h2>");
  }
  finish();

  return 0;
}
