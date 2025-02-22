#include <WiFi.h>
#include "configuration.h"
#include "station_utils.h"
#include "aprs_is_utils.h"
#include "lora_utils.h"
#include "digi_utils.h"
#include "wifi_utils.h"
#include "gps_utils.h"
#include "display.h"
#include "utils.h"

extern Configuration    Config;
extern uint32_t         lastScreenOn;
extern String           iGateBeaconPacket;
extern String           firstLine;
extern String           secondLine;
extern String           thirdLine;
extern String           fourthLine;
extern String           fifthLine;
extern String           sixthLine;
extern String           seventhLine;


namespace DIGI_Utils {

    String generateDigiRepeatedPacket(String packet, String callsign) {
        String sender, temp0, tocall, path;
        sender = packet.substring(0,packet.indexOf(">"));
        temp0 = packet.substring(packet.indexOf(">")+1,packet.indexOf(":"));
        if (temp0.indexOf(",") > 2) {
            tocall = temp0.substring(0,temp0.indexOf(","));
            path = temp0.substring(temp0.indexOf(",")+1,temp0.indexOf(":"));
            if (path.indexOf("WIDE1-")>=0) {
                String hop = path.substring(path.indexOf("WIDE1-")+6, path.indexOf("WIDE1-")+7);
                if (hop.toInt()>=1 && hop.toInt()<=7) {
                    if (hop.toInt()==1) {
                        path.replace("WIDE1-1", callsign + "*");
                    } else {
                        path.replace("WIDE1-" + hop , callsign + "*,WIDE1-" + String(hop.toInt()-1));
                    }
                    String repeatedPacket = sender + ">" + tocall + "," + path + packet.substring(packet.indexOf(":"));
                    return repeatedPacket;
                } else {
                    return "";
                }
            } else {
                return "";
            }
        } else {
            return "";
        }
    }

    void processPacket(String packet) {
        String loraPacket;
        if (packet != "") {
            Serial.print("Received Lora Packet   : " + String(packet));
            if ((packet.substring(0, 3) == "\x3c\xff\x01") && (packet.indexOf("NOGATE") == -1)) {
                Serial.println("   ---> APRS LoRa Packet");
                String sender = packet.substring(3,packet.indexOf(">"));
                STATION_Utils::updateLastHeard(sender);
                STATION_Utils::updatePacketBuffer(packet);
                Utils::typeOfPacket(packet.substring(3), "Digi");
                if (packet.indexOf("WIDE1-") > 10 && Config.digi.mode == 2) { // If should repeat packet (WIDE1 Digi)
                    loraPacket = generateDigiRepeatedPacket(packet.substring(3), Config.callsign);
                    if (loraPacket != "") {
                        delay(500);
                        Serial.println(loraPacket);
                        LoRa_Utils::sendNewPacket("APRS", loraPacket);
                        display_toggle(true);
                        lastScreenOn = millis();
                    }
                }
            } else {
                Serial.println("   ---> LoRa Packet Ignored (first 3 bytes or NOGATE)\n");
            }
        }
    }

    void loop(String packet) {
        processPacket(packet);
    }

}