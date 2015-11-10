import java.io.*;
import java.util.*;
import jssc.*;

public class COMunication {
	
	private static SerialPort serialPort;
	private static int baudRate;
	private static byte[] data;
	private static boolean newData;
	private static boolean isSetup = false;

	//initialize the comstuff.
	public void setup(String port, int baudRate_){
		serialPort = new SerialPort(port);
		baudRate = baudRate_;
		isSetup = true;
		 try {
	            serialPort.openPort();//Open port
	            serialPort.setParams(9600, 8, 1, 0);//Set params
	            int mask = SerialPort.MASK_RXCHAR;//Prepare mask, allows evenlistener on incoming data
	            serialPort.setEventsMask(mask);//Set mask
	            serialPort.addEventListener(new SerialPortReader());//Add SerialPortEventListener
	        }
	        catch (SerialPortException ex) {
	            System.out.println(ex);
	        }
	}
	
	// returns true is the comunication is up and running
	public boolean isSetup(){return isSetup;}
	
	//returns the byte new data, if data is available send it to the gui
	public boolean isDataAvailable(){ return newData; }
	
	//Delivers the data to the gui and sets new data to false
	public byte[] deliverData(){
		newData = false;
		return data;
	}
	
	// sends data over comport
	public void sendData(byte[] dataToSend){
        try {
            serialPort.openPort();//Open serial port
            serialPort.setParams(baudRate, 
        			SerialPort.DATABITS_8,
        			SerialPort.STOPBITS_1,
        			SerialPort.PARITY_NONE);//Set parameters. 
            serialPort.writeBytes(dataToSend);//Write data to port
            serialPort.closePort();//Close serial port
        }
        catch (SerialPortException ex) {
            System.out.println(ex);
        }
	}
	
	//Recieve data over comport.
	private static byte[] receiveData(){
		byte[] data1, data2, dataOut;
		data1 = new byte[0];
		data2 = new byte[0];
		dataOut = new byte[0];
		try {
            serialPort.openPort();//Open serial port
            serialPort.setParams(baudRate, 
                    			SerialPort.DATABITS_8,
                    			SerialPort.STOPBITS_1,
                    			SerialPort.PARITY_NONE);//Set parameters. 
            data1 = serialPort.readBytes(1);//Read 1 byte from serial port
            
            if((data1[0] >>> 4) != 0)// shift to get the number of incoming bytes. If we need to recive more bytes we get to know it here. 
            	data2 = serialPort.readBytes(data1[0] >>> 4); //Read the rest of the bytes.
            
            serialPort.closePort();//Close serial port
        }
        catch (SerialPortException ex) {
            System.out.println(ex);
        }
		
		//merge first byte with the possible rest of data to get it all in one array
		dataOut = new byte[data1.length + data2.length];
        dataOut[0] = data1[0];
        for (int i = 0; i < data2.length; i++)
        	dataOut[i+1] = data2[i];
        return dataOut;
	}
	
	//Lists all the active ports, Will be used in GUI to select the port we want. 
	public String[] availablePorts() {return SerialPortList.getPortNames();}
	
	static class SerialPortReader implements SerialPortEventListener {

	//EventListener that waits for incoming data.
    public void serialEvent(SerialPortEvent event) {
        if(event.isRXCHAR()){//If data is available
            data = receiveData();
            newData = true; //set variable that new data is available for GUI
        }
    }
}}
