#include <RCTrx.hpp>
#include <algorithm>
#include <ssca.h>

int readLine(char *buffer, int len);

long times[] = {  // proove1 A on 1
1122, 101222, 1112, 102256, 1120, 101241, 1112, 31074, 307, 69820, 1120, 101278, 1112, 101253,
1120, 6949, 418, 1044, 416, 1037, 420, 1036, 416, 1037, 418, 1045, 417, 1039, 415, 1038, 415,
1042, 415, 1045, 413, 1042, 416, 1041, 413, 1041, 414, 1049, 411, 1044, 411, 1047, 406, 1051,
405, 1051, 411, 1043, 412, 1045, 1129, 339, 405, 1058, 1123, 345, 400, 1056, 1118, 352, 396,
11457, 413, 1048, 414, 1043, 411, 1044, 414, 1041, 411, 1052, 410, 1044, 411, 1044, 410, 1045,
411, 1054, 407, 1046, 410, 1048, 407, 1047, 408, 1052, 407, 1049, 408, 1049, 407, 1046, 409,
1053, 409, 1048, 407, 1050, 1124, 343, 402, 1061, 1118, 353, 394, 1059, 1117, 353, 391,
11469, 408, 126, 1121, 1271, 350, 1106, 350, 1106, 351, 1109, 353, 1105, 351, 1102, 354, 1103,
353, 1107, 358, 1097, 357, 1098, 359, 1098, 357, 1104, 360, 1097, 359, 1097, 360, 1096, 360,
1100, 362, 1095, 361, 1093, 1085, 386, 360, 1103, 1089, 379, 361, 1097, 1080, 387, 360,
11497, 376, 1086, 377, 1077, 379, 1080, 376, 1077, 378, 1087, 378, 1077, 378, 1075, 383, 1074,
380, 1082, 381, 1074, 384, 1074, 383, 1072, 383, 1078, 385, 1070, 387, 1075, 381, 1071, 386,
1076, 387, 1068, 387, 1069, 1107, 362, 383, 1081, 1105, 367, 379, 1076, 1101, 367, 380,
11478, 399, 1065, 399, 1058, 397, 1057, 398, 1057, 400, 1063, 399, 1058, 404, 76, 1120, 1325,
345, 1118, 345, 1109, 348, 1109, 348, 1108, 348, 1115, 350, 1103, 353, 1104, 353, 1100, 356,
1106, 358, 1100, 356, 1096, 1083, 390, 356, 1107, 1079, 389, 358, 1100, 1079, 389, 357,
11508, 368, 1088, 376, 1081, 375, 1078, 378, 1081, 375, 1088, 376, 1079, 379, 1076, 381, 1076,
380, 1081, 382, 1077, 379, 1074, 383, 1074, 383, 1080, 384, 1071, 386, 1072, 385, 1069, 386,
1081, 383, 1068, 390, 1068, 1108, 360, 386, 1079, 1106, 363, 383, 1075, 1103, 369, 377,
11482, 398, 1064, 400, 1056, 400, 1057, 400, 1058, 400, 1060, 401, 1055, 403, 1056, 400, 1054,
401, 1063, 402, 1052, 405, 1052, 402, 1056, 405, 211, 1119, 1201, 341, 1111, 345, 1109, 349,
1114, 349, 1107, 351, 1104, 1075, 394, 352, 1111, 1076, 395, 351, 1108, 1072, 397, 351,
11508, 369, 1103, 361, 1084, 374, 1082, 374, 1082, 375, 1088, 376, 1082, 374, 1080, 377, 1081,
376, 1087, 378, 1076, 381, 1078, 378, 1077, 380, 1081, 384, 1073, 384, 1071, 386, 1070, 386,
1079, 385, 1070, 387, 1071, 1107, 363, 383, 1081, 1104, 365, 382, 1074, 1105, 369, 377};
int timesLength = sizeof times / sizeof times[0];

RCTrx rxtrx;

long i = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }
  delay(100);
  Serial.println();
  Serial.println("ready.");
  rxtrx = RCTrx();
  rxtrx.onCodeReceived([](Code code){
    Serial.printf("Received code %lu\n", code);
  });
 // rxtrx.enableReceive(4);
 rxtrx.setSendPin(14);
 // rxtrx.sendTimeArray(times, timesLength);
}

char line[100];
unsigned long code;
int protocolId;

void loop() {
  if (readLine(line, 100)) {
    if (sscanf(line, "send %lu %d", &code, &protocolId) == 2) {
      rxtrx.send(code, protocolId);
    } else if (strcmp(line, "rec") == 0) {
      rxtrx.enableReceive(4);
    } else if (sscanf(line, "sniff %lu %d", &code, &protocolId) == 2) {
      //rxtrx.sniff(4)
    } else {
      Serial.printf("send <code> <protocolId> - send code using specified protocol on pin D5\n");
      Serial.printf("rec - enable receiver on pin D2\n");
      Serial.printf("sniff - reads and lists state and duration of 200 radio pulses\n");
    }
  }
}

int readLine(char *buffer, int len) {
  static int pos = 0;

  int readch = Serial.read();
  if (readch > 0) {
    switch (readch) {
    case '\n': // Ignore new-lines
      break;
    case '\r': // Return on CR
      pos = 0;  // Reset position index ready for next time
      Serial.println();
      return 1;
    default:
      Serial.write(readch);
      if (pos < len-1) {
        buffer[pos++] = readch;
        buffer[pos] = 0;
      }
    }
  }
  return 0;
}
