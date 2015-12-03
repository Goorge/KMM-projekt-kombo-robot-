import jssc.*;

public class COMunication {
	
	private static SerialPort serialPort;
	private static int baudRate;
	private static byte[] data_from_bt;
	private boolean newData = false;
	private boolean isSetup = false;
	
	//initialize the comstuff.
	public void setup(String port, int baudRate_){
		try {
			if(isSetup)
				if(serialPort.isOpened())
					serialPort.closePort();
			serialPort = new SerialPort(port);
			baudRate = baudRate_;
			isSetup = true;
			serialPort.openPort();//Open port
	        serialPort.setParams(baudRate, 8, 1, 0);//Set params
	        int mask = SerialPort.MASK_RXCHAR;//Prepare mask, allows evenlistener on incoming data
	        serialPort.setEventsMask(mask);//Set mask
	        serialPort.addEventListener(new SerialPortReader());//Add SerialPortEventListener
		}
	    catch (SerialPortException ex) {
	    	System.out.println(ex);
	    }
	}
	
	public void clearDataAvailable(){ newData = false; }
	
	// returns true is the comunication is up and running
	public boolean GetIsSetup(){ return isSetup; }
	
	//returns the byte new data, if data is available send it to the gui
	public boolean isDataAvailable(){ return newData; }
	
	//Delivers the data to the gui and sets new data to false
	public byte[] deliverData(){
		newData = false;
		return data_from_bt;
	}
	
	// sends data over comport
	public void sendData(byte[] dataToSend){
        try {
            serialPort.setParams(baudRate, 
        			SerialPort.DATABITS_8,
        			SerialPort.STOPBITS_1,
        			SerialPort.PARITY_NONE);//Set parameters. 
            serialPort.writeBytes(dataToSend);//Write data to port
        }
        catch (SerialPortException ex) {
            System.out.println(ex);
        }
	}
	
	public void disconnect(){
		if(isSetup){
			if(serialPort.isOpened()){
				try {
					serialPort.closePort();
				} catch (SerialPortException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}
	
	//Recieve data over comport.
	private byte[] receiveData(){
		byte[] data1, data2, dataOut;
		data1 = new byte[1];
		data2 = new byte[0];
		dataOut = new byte[0];
		try {
            serialPort.setParams(baudRate, 
                    			SerialPort.DATABITS_8,
                    			SerialPort.STOPBITS_1,
                    			SerialPort.PARITY_NONE);//Set parameters. 
            
            data1 = serialPort.readBytes(1);//Read 1 byte from serial port
            //for(int i = 1; i < ((data1[0] >>> 4)&0x0f); i++)// shift to get the number of incoming bytes. If we need to recive more bytes we get to know it here.
            data2 = serialPort.readBytes(((data1[0] >>> 4)&0x0f)); //Read the rest of the bytes.
        }
        catch (SerialPortException ex) {
            System.out.println(ex);
        }
		
		//merge first byte with the possible rest of data to get it all in one array
		dataOut = new byte[data1.length + data2.length];//data1.length + data2.length];
        dataOut[0] = data1[0];
        for (int i = 0; i < data2.length; i++){
        	dataOut[i+1] = data2[i];
        }
        return dataOut;
	}
	
	//Lists all the active ports, Will be used in GUI to select the port we want. 
	public String[] availablePorts() {return SerialPortList.getPortNames();}
	
	class SerialPortReader implements SerialPortEventListener {

	//EventListener that waits for incoming data.
		public void serialEvent(SerialPortEvent event) {
        	if(event.isRXCHAR()){//If data is available
            	data_from_bt = receiveData();
            	newData = true;//set variable that new data is available for GUI
        	}
    	}
	}
}
