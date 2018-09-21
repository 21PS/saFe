# saFe
A small effort aimed at women's safety solutions. A hand-held device scans user's fingerprint periodically, any situation harming the user may interrupt the fingerprint scanning & triggers the loud audio alert & message alert with current location.

  This project consists of two sections; A compact fingerprint scanning section which is to be held in hand & an alert section which can be placed in user's bag. The two sections are connected over WiFi. The fingerprint scanning section comprises of a fingerprint scanner module, microcontroller with inbuilt WiFi, rechargeable battery & battery charging circuit. The alert section comprises of a microcontroller with inbuilt WiFi, gsm-gprs-gps module, mp3 player module, amplifier, speaker, rechargeable battery & battery charging circuit.
  
  The fingerprint scanning section periodically scans for the user's fingerprint and acknowledges the status to the alerting section. If the fingerprint is read & authenticated, status is set as 1. If the fingerprint is read & not authenticated, status is set as 2. If the no fingerprint is found, status is set as 3. If the status is 1, the system continues in the normal state; if the status is 2 or 3, the system initializes a counter and monitors the count. If the count become 5 ie; fingerprint not authenticated for a preset period, a message alert with current location & a loud audio alert is initiated by the system helping in immediate attention of nearby people through audio & user's relatives through message alerts.
 
Project demo video: https://www.youtube.com/watch?v=Kvhu4DU9Gv0
