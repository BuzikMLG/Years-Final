#include "MagStripe.h"

MagStripe card;

static const byte DATA_BUFFER_LEN = 108;
static char data[DATA_BUFFER_LEN];

void setup()
{
  Serial.begin(9600);
  card.begin(2);
}

 
void loop()
{
  if (!card.available()) {
    return;
  }
  short chars = card.read(data, DATA_BUFFER_LEN);
  if (chars < 0) {
    return;
  }

  Serial.println(data);
}
