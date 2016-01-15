void process_data(){
  float gesturePoints[NUM_STATES][2];
  float gestureDist[2];
  
  float totalDist = 0;
  int currentMax = 0;
  float currentMaxValue = -1;

  int maxY = 0;
  int maxI = 0;
  for (int i=0; i<sizeof(freq); i++) {
    if (maxY < results[i])
    {
      maxY = results[i];
      maxI = i;
    }
  }

  int n_button_state = digitalRead(NOTHING_BUTTON_PIN);
  if (n_button_state == HIGH){
    Serial.println("clicked nothing!");
    analogWrite(NOTHING_LED_PIN, 1); 
    gesturePoints[NOTHING_INDEX][0] = freq[maxI];
    gesturePoints[NOTHING_INDEX][1] = results[maxI];
  }
  
  int a_button_state = digitalRead(ADULT_BUTTON_PIN);
  if (a_button_state == HIGH) {
    Serial.println("clicked adult!");
    analogWrite(ADULT_LED_PIN, 1); 
    gesturePoints[ADULT_INDEX][0] = freq[maxI];
    gesturePoints[ADULT_INDEX][1] = results[maxI];
  }

  int c_button_state = digitalRead(CHILD_BUTTON_PIN);
  if (c_button_state == HIGH) {
    Serial.println("clicked child!");
    analogWrite(CHILD_LED_PIN, 1); 
    gesturePoints[CHILD_INDEX][0] = freq[maxI];
    gesturePoints[CHILD_INDEX][1] = results[maxI];
  }
  
  for (int i = 0; i < NUM_STATES; i++)
  {
    //calculate distance between two points
    float x1 = freq[maxI];
    float y1 = results[maxI];
    float x2 = gesturePoints[i][0];
    float y2 = gesturePoints[i][1];

    float distance_x = x2-x1;
    float distance_y = y2-y1;

    gestureDist[i] = sqrt((distance_x * distance_x) + (distance_y * distance_y));

    totalDist = totalDist + gestureDist[i];
    if(gestureDist[i] < currentMaxValue || i == 0)
    {
      currentMax = i;
      currentMaxValue = gestureDist[i];
    }
  }
  totalDist=totalDist/3;

  for (int i = 0; i < NUM_STATES;i++)
  {
    float currentAmount = 0;
    currentAmount = 1-gestureDist[i]/totalDist;
    if(currentMax == i && currentAmount > 0.85f)
    {
      Serial.print("holder index: ");
      Serial.println(holderIndex,DEC);
      switch (i) {
        case NOTHING_INDEX:
          Serial.println("Nothing is holding!");
          analogWrite(LED_PIN, 1);
          holderIndex = NOTHING_INDEX;
          break;
        case ADULT_INDEX:
          analogWrite(LED_PIN, 0);
          Serial.println("Adult is holding!");
          if (holderIndex == ADULT_INDEX){
            if (lastMelody != ADULT_INDEX){
              holderCount++;
              if (holderCount > HOLD_CONFIRM){
                play_mario();
                lastMelody = ADULT_INDEX;
              }
            }
          } else {
            holderCount = 0;
            holderIndex = ADULT_INDEX;
          }
          break;
        case CHILD_INDEX:
          analogWrite(LED_PIN, 0);
          Serial.println("Child is holding!");
          if (holderIndex == CHILD_INDEX){
            if (lastMelody != CHILD_INDEX){
              holderCount++;
              if (holderCount > HOLD_CONFIRM){
                play_alarm();
                lastMelody = CHILD_INDEX;
              }
            }
          } else {
            holderCount = 0;
            holderIndex = CHILD_INDEX;
          }
          break;
        default: 
          Serial.println("Shouldn't ever be here!");
          break;
      }
    }
  }
}
