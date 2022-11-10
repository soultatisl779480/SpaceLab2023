#include <DFRobot_EOxygenSensor.h>


/*********************************************************************
 *    Arduino software to interface a text file to the Quest Institute
 *    MicroLab.  This program along with the MicroLab program will
 *    add text to the end of the default text file that is generated 
 *    eveytime a photo is taken.  This Text function is added to the 
 *    MicroLab code after the default text is generated for the photo.
 *    Therefore, the request for Arduino text will be generated during
 *    the period before the photo is taken. So, during the TakePhoto
 *    MicroLab process the Arduino text is placed at the end of the 
 *    default text file.  Note: a total of 900 charators can be added
 *    to the text file.
 *    
 *      1.  this interface uses Software Serial in the Arduino to 
 *          communicate with the MicroLab.  Therefore, Any Arduino
 *          can be used for this purpose.  No need for a dedicated
 *          Serial Communication line, although the program could be
 *          modified to do so.
 *       2. Communication between the Arduino and MicroLab is limited
 *          to 9600 Baud due to the speed and serial interface or the 
 *          Parallax Stamp microcontroller. 
 *          
 *    Hardware connections in the stock program are:
 *    Transmit from Arduino (2) (TX)====> (Rx) SDA (8) MicroLab
 *    Receive from Microlab (3) (Rx)<==== (Tx) SCL (9) MicroLab
 *    Note: the pins may be changed in the program below to accommodate 
 *    the Arduino being used.
 *    
 *                   Description of Operation:
 *    ARDUINO:
 *    The Arduino software sets up a Text Buffer 1024 charators in size
 *    Ascii charators are entered into this buffer by the Function -
 *            arrayprint(uint8_t x);
 *    This function places the ascii charator x into the next position 
 *    in the Buffer.  The position is determined by the arraypointer.
 *    the arraypointer is automatically incremented to the next location and 
 *    an End of File marker is placed in that next location.  The End of 
 *    file marker (0xFF) is used during the transfer Function to indicate
 *    when to stop the transfer.  The MicroLab is also looking for this 
 *    marker to know when the transfer is complete.  If the MicroLab 
 *    does not receive the End of File marker the text transfer is timeed out
 *    and aborted.
 *    The writing to the text buffer by the Arduino can occure at any
 *    time.  However, in the Arduino main code loop a call to Function -
 *            CheckforText();         
 *    This function checks for a Serial command input indicating the 
 *    MicroLab is requesting the text file, This command ia a (0x5A)
 *    hex code.  The MicroLab will send this command to the Arduino and 
 *    wait a total of 2000 milli seconds for a response, if no response
 *    is received from the Arduino, the request is aborted and a blank
 *    text file is appended to the default text file generated when a 
 *    photo is taken.  Note: a total of 900 Ascii Charators can be added
 *    to the text file.
 *    
 *    MICROLAB
 *    Software must be added to the MicroLab to make this Text operation 
 *    possible.  The software can be added to any of te stock templetes.
 *    in slot 4 at line approximally 576 -  See statement:
 *    '
 *    'To Add additional Text to the Text output file => Add it here
 *    '
 *      MICROLAB CODE STARTS HERE
    '
    '******************************************************************************
    '******* NEW For adding text data output for each photo event *****************
    '******************************************************************************
    '
    ' This NEW20221028 Routine is used to add text to end of the Default text file
    '         Total 1020 charators can be added, but must end with eof "0xFF"
    '         Any External processor can be used to Send and receive text data
    '
    '---For the Microlab input and output to the External processor:

    '       Serial INPUT uses "SERIN payloadsda,$00F0,2000,TimeoutT,[TempReg1]" '
    '          This command will set the payloadsda(P8) as a Serial input at 9600 baud
    '          if no input is received within 2000millisec the command will abort and
    '          go to TimeoutT routine.  If not timmed out the incomming serial data
    '          will be in the "TempReg1" register.
    '
    '        Serial OUTPUT uses "SEROUT payloadscl,$00F0,[Ack]"  '
    '           This command will set the payloadscl(P9) as a Serial output at 9600 baud
    '           The (Ack) value (0x5A) is the commant that is sent to the External Processor
    '           The External processer will respond with the text to be added to the
    '           end of the default text fild during a Takephoto event.
    '
'----------------------------------------------------------------------------------------
'----------------------------------------------------------------------------------------
    TempReg1 = $0D             'Load a carrage return charator, for seperation between default text
    I2COUT InternI2c, $A0 | TempReg2,xcounter.HIGHBYTE\xcounter.LOWBYTE, [TempReg1]  'write charator in ee2ram
    PAUSE 10                   'Pause for the time to write to eerom
    xcounter = xcounter +1     'point to the next location
    TempReg1 = $0A             'Load a line feed for serperation between default text and new text
    I2COUT InternI2c, $A0 | TempReg2,xcounter.HIGHBYTE\xcounter.LOWBYTE, [TempReg1]  'write charator in ee2ram
    PAUSE 10                    'Pause for the time to write to eerom
    xcounter = xcounter +1      'point to the next location
    '
    '
    SEROUT payloadscl,$00F0,[Ack]                   'Send request to External processor for additional Text
A_data:                                             'Note: the 10millisec pause is built into the sender rate
    SERIN payloadsda,$00F0,2000,TimeoutT,[TempReg1]  'Wait for Text to begin to be received
    I2COUT InternI2c, $A0 | TempReg2,xcounter.HIGHBYTE\xcounter.LOWBYTE, [TempReg1]  'write charator in ee2ram
    xcounter = xcounter + 1                          'point to next location
    IF xcounter > ($C000+1020+580) THEN TimeoutT     'check for maximum text size Default + New text maximum
    IF NOT (TempReg1 = $FF) THEN A_data              'Not end of file yet, get another text charator
    GOTO Gotit                                       'got end of text marker goto finish
TimeoutT:                                       'Text input timed out, abort receiption
    DEBUG CR,"Text abort",CR                    'send Abort to terminal
    I2COUT InternI2c, $A0 | TempReg2,xcounter.HIGHBYTE\xcounter.LOWBYTE, [$FF]  'forced end of file marker
Gotit:
    DEBUG CR,"AT",CR        'Send "AT" to the terminal to indiate the text transfer is complete
'-------------------------------------------------------------------------------
'-------------------------------------------------------------------------------
    ' END of MICROLAB TEXT 20221029 Routine
    '
*/
//
 //
 //************ ARDUINO TEXT TEST CODE STARTS HERE ***************************
 //
 //------   REQUIRED information for this operation   -----------
 //
#include <SoftwareSerial.h>
//
SoftwareSerial mySerial(3, 2);    // RX pin used on arduino, TX pin used on arduino
#define RequestData 0x5A          //command request from microlab to send text data
#define maxbuffer   1020          //maximum number of charators in text buffer
uint8_t TextReady = 0;            //0=text not ready to be taken, 1=text ready to be taken
long arraypointer = 0;            //current pointer into text buffer
char ArduinoData[1024] = "\xFF";  //Text buffer holds text to be sent to the microlab
//
#include "DFRobot_OxygenSensor.h"
#define COLLECT_NUMBER    10             // range: 1-100.
#define Oxygen_IICAddress ADDRESS_3
//---- Function Protypes to transfer text data from Arduino to MicroLab ----
//
void CheckforText(void);              //Routine to check for text request                             //
void arrayprint(uint8_t x);           //write a char to the text buffer one at a time
//
//-------------------------------------------------------------
//            USED ONLY FOR TESTING
//
long oldmillis = 0;                 //used for storing to old millis count
uint8_t simtext = ' ';              //simulated text value
//
//-------------------------------------------------------------
//    Testing Sumulation setup
//
DFRobot_OxygenSensor Oxygen;
void setup()
{  
  Serial.begin(9600);       // NOT required   Open serial communications and wait for port to open:
//  while (!Serial) {}      // NOT required   Wait for usb
  delay(100);               // NOT required
//
  mySerial.begin(9600);     // REQUIRED in setup data rate for the SoftwareSerial port to MicroLab
}
//-------------------------------------------------------------
//
//
//////////////////////////////////////////////////////////////
//////  Loop for testing / Simulation ////////////////////////
//////////////////////////////////////////////////////////////
//
void loop()                      // Simulation of Arduino main loop
{
//
  if(TextReady == 1){             //this checks if text is complete and ready
      CheckforText();             //Begin checking to transfer text to the microlab
  }                               //photo transfer/text request from microlab
//i2c code
  float o2 = Oxygen.getOxygenData(COLLECT_NUMBER);
  arrayprint("O2 level is ");
  arrayprint(o2);
  TextReady = 1;
  CheckforText();
  delay(1000);
//
//-----------------------------------------------------------------  
//  BELOW IS A TEST OPERATION SIMULATING TEXT BEING ADDED TO BUFFER
//-----------------------------------------------------------------
//
    if (TextReady == 0){                              //Time to add Text for next photo event
              uint16_t y = random(maxbuffer-1);       //This tests for a random number of text charators
               if((millis() - oldmillis)>100){        //This simulate time between writes to text buffer
                  oldmillis = millis();               //reset oldtime to now time for next charator write
                  for(uint16_t x = 0;x < y;x++){      //this controlles the number for charators written
                      arrayprint(simtext);            //place the charator into the text buffers
                      simtext++;                      //inc to the next ascii charator
                      if(simtext>0x7F){               //is it the last printable ascii charator
                        simtext = ' ';                //it was the last printable, now start over
                        arrayprint('\r');             //do a carrage return
                        arrayprint('\n');             //do new line
                      }                               //finish new line of text
                  }                                   //finish writting charators into the text buffer
                  TextReady = 1;                      //set text complete flag for output Function
                  Serial.print("\r\ntext ready ");    //Send new text ready for next photo event 
                  Serial.println(y);                  //State the number of charators in text buffer    
              }         
    }
//--------------------------------------------------------------------------------------------
//--- END OF SIMULATED TEST OF CHARATORS BEING SENT TO TEXT BUFFER ---------------------------
//--------------------------------------------------------------------------------------------
}
//
///////////////////////////////////////////////////////////////////////////
/////// End of Loop for testing / Simulation //////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
//----------------------------------------------
//  Function to add a charator to the text buffer
//
//  this routine overwrites the end of text marker(0xFF)
//  with the value that was sent to this function in x
//  then it will place a new end of text marker(0xFF) at
//  the end of the valid text buffer.  this operation
//  repeats with every charator sent to this routine
//  therefore, always keeping the buffer valid.
//
//----------------------------------------------
//
void arrayprint(uint8_t x){                       //Function entry with charator in x
      ArduinoData[arraypointer]=x;                //place x in buffer pointed to by arraypointer
      Serial.write(x);              //OPTIONAL TO OUTPUT DATA WRITTEN INTO TEXT BUFFER, REMOVE FOR FLIGHT
      arraypointer++;                             //move pointer to next location in buffer
      if(arraypointer > maxbuffer){               //check for maximum number of charators in buffer
        arraypointer = maxbuffer;                 //if over maximum reset to maximum  
      }                                           //now arraypointer point to next location
      ArduinoData[arraypointer]=0xFF;             //always put end of text marker at the end of file
    }                                             //Function finished
  
//----------------------------------------------
//  Function to transfer text to the microlab
// 
//  this checks for a text transfer command from the microlab
//  if no command received, it will just return.  The microlab is waiting for the tranfer text command, if 
//  none is sent from the microlab, the function will abort the request and continue
//  its operations and will request later.  The request from the 
//  microlab is generated every takephoto event.
//
//----------------------------------------------
//
void CheckforText(void){                        //entry to check for text request
  if (mySerial.available()){                    // was the request received ?
    uint8_t y =(mySerial.read());               // yes, get the request byte
      if (y = RequestData) {                    // check if request is valid
        delay(15);                              // delay 15 millisec to sync with microlab
        arraypointer = 0;                       // point text pointer to the beginning of array
        uint8_t x =' ';
        do{                                       // if value fetched is not 0xFF do it again
            x=(ArduinoData[arraypointer]);  // do this - get value from text buffer into x
            mySerial.write(x);                  // send that value to the microlab
            delay(10);                           // 10 millisec for microlab to process and store
            arraypointer++;                     // point text pointer to the next location
        }while((x) != 0xFF);                    // value fetched is 0xFF - transfer finished
        TextReady = 0;                         // Text transfered reset to get new text
        arraypointer =0;                       // reset text pointer, text read
      }
      else {                                    //if it was not a request text command abort
      Serial.println ("\r\n unknown command");  //output to arduino terminal bad request abort
      }                                         //Transfer finished 
   }                                            //check availiable finished
}                                               //Function finished
