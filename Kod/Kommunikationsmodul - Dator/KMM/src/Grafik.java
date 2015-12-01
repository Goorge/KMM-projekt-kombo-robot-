
import java.awt.EventQueue;
import javax.swing.JFrame;
import javax.swing.ScrollPaneConstants;

import javax.swing.DefaultListModel;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.util.concurrent.TimeUnit;
import java.awt.event.ActionEvent;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import java.awt.Color;

import javax.swing.JList;
import java.awt.Button;
import javax.swing.JLabel;

import java.awt.event.MouseListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

enum listEnum{leftDistance, rightDistance, frontDistance, gyro, leftWheel, rightWheel, line, rgb, garbage};
enum directions{goForwards, goBackwards, goLeft, goRight, turnLeft, turnRight, StandStill}; 

public class Grafik {
	// The frame that everything gets drawn into.
	private JFrame frame;
	
	// buttons 4 distance sensors
	private static Button sensor_front_btn;
	private static Button sensor_left_btn;
	private static Button sensor_right_btn;
	
	// button 4 gyro
	private static Button gyro_btn;
	
	// buttons 4 wheels
	private static Button wheel_right_btn;
	private static Button wheel_left_btn;
	
	// button 4 line
	private static Button line_btn;
	
	// button 4 rgb
	private static Button rgb_btn;
	
	//buttons stearing
	private JButton go_right_btn;
	private JButton go_left_btn;
	private JButton go_forward_btn;
	private JButton turn_right_btn;
	private JButton go_back_btn;
	private JButton turn_left_btn; 
	
	// THE LISTs
	private static JList graficList;
	private static JPanel panel = new JPanel();
	private static JScrollPane listScroll = new JScrollPane();
	private static DefaultListModel[] lists;
	private static DefaultListModel comPorts;
	
	// Labels
	private static JLabel line_labels[]; // Used to show a grafical version of the different line sensors 
	private JLabel image_label;	  // Used for the background image
	private static JLabel rgb_label;	  // Use to show if the RGB is getting any data  
	private JLabel battery_label;
	private JLabel battery_text_label;
	//SPPPEEEEDDD of the bluetooth
	private static int baudRate = 115200;
	
	//Bluetooth thingy
	private static COMunication com = new COMunication();
	
	//bool for buttonspress
	private boolean is_button_pressed = false;
	
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					Grafik window = new Grafik();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		boolean test;
		while(true){
			test = com.GetIsSetup();
			System.out.print("");
			if(test){
				//System.out.println("Klar med setup");
				if(com.isDataAvailable()){
					System.out.println("Mottagen data");
					handleData(com.deliverData());
				}
			}
		}
	}
	
	private static void changeRGBColor(Color color){ rgb_label.setBackground(color);}
	
	// Tar hand om data som kommer ifr�n bt, Beh�ver g�ras mer. 
	private static void handleData(byte[] data){
		switch (data[0] & 0x0f){
		case (byte)0x00: // Batteri
			// V�rdet / 51.2
			break;
		case (byte)0x01: // Avst�ndssensorer
			// v�nster, rakt fram, h�ger
			addToList(listEnum.leftDistance, getCurrentTime() + data[1]);
			setButtonLabel(listEnum.leftDistance, Byte.toString(data[1]) );
			addToList(listEnum.rightDistance, getCurrentTime() + data[2]);
			setButtonLabel(listEnum.rightDistance, Byte.toString(data[2]));
			addToList(listEnum.frontDistance, getCurrentTime() + data[3]);
			setButtonLabel(listEnum.frontDistance, Byte.toString(data[3]));
			break;
		case (byte)0x02: //Linjesensorer
			String text = getCurrentTime() + ": "; //
			Color color_to_line[] = new Color[11];
			for(int i = 3; i >= 1; i--)
				for(int j = 3; j >= 0; j--){
					byte current_data = (byte) ((data[i] >>> (j*2)) & 0x03);
					if(!(i == 3 && j == 3)){
						text += current_data + " ";
						Color color[] = { Color.black, Color.darkGray, Color.lightGray, Color.white };
						color_to_line[(3-j) + (3-i) * 4 -1] = color[current_data];
					}
				}
			addToList(listEnum.line, text);
			setLineBackground(color_to_line);
			break;
		case (byte)0x03: // RGB BL� 
			addToList(listEnum.rgb, getCurrentTime() + "R�D");
			changeRGBColor(Color.RED);
			break;
		case (byte)0x04: // RGB GR�N 
			addToList(listEnum.rgb, getCurrentTime() + "GR�N");
			changeRGBColor(Color.GREEN);	
			break;
		case (byte)0x05: // RGB BL� 
			addToList(listEnum.rgb, getCurrentTime() + "BL�");
			changeRGBColor(Color.BLUE);
			break;
		case (byte)0x06: // gyro klart 
			addToList(listEnum.gyro, getCurrentTime() + "GyroKlart");
			break;
		case (byte)0x07: // gyro aktiverat 
			addToList(listEnum.gyro, getCurrentTime() + "GyroStart");
			break;		
		default: // If we dont get any real data, we still have a list where we write it so we know if it got wrong
			String text2 = ""; 
			for(int i = 0; i < data.length; i++){
				text2 += data[i];
				System.out.println(" " + data[i]);
			}
			addToList(listEnum.garbage, getCurrentTime() + " " + text2);
			break; 
		}
		com.clearDataAvailable();
	}
	
	// label buttons
	public static void setButtonLabel(listEnum button, String value){
		switch (button){
		case leftDistance:  
			sensor_left_btn.setLabel(value);
			break;
		case rightDistance: 
			sensor_right_btn.setLabel(value);
            break;
		case frontDistance: 
			sensor_front_btn.setLabel(value);
            break;
		case gyro:  		
			gyro_btn.setLabel(value);
            break;
		case rightWheel:  	
			wheel_right_btn.setLabel(value);
            break;
		case leftWheel:  	
			wheel_left_btn.setLabel(value);
            break;
		case line:  		
			line_btn.setLabel(value);	
            break;
		default:  
            break;
		}
	}

	//Skicka data via bt, Beh�ver uppdateras med mer data. 
	private void sendDirection(directions dirr){
		byte toSend[] = new byte[1];
		switch (dirr){
		case goForwards: 
			toSend[0] = (byte)0x09; //00001001
			break;
		case goBackwards:
			toSend[0] = (byte)0x0A; //00001010
			break;
		case goLeft://?
			toSend[0] = (byte)0x0B; //00001011
			break;
		case goRight://?
			toSend[0] = (byte)0x0C; //00001100
			break;
		case turnLeft:
			toSend[0] = (byte)0x0D; //00001101
			break;
		case turnRight:
			toSend[0] = (byte)0x0E; //00001110
			break;
		case StandStill:
			toSend[0] = (byte)0x08; //00001000
			break;
		default:
			break;
		} 
		if(com.GetIsSetup())
			com.sendData(toSend);
	}
	
	// Select what data to write to the list
	private void setJListVisible(listEnum list){
		graficList.setModel(lists[list.ordinal()]);
	}
	
	// Set color on rgb_label
	public void setRGBBackground(Color color){
		rgb_label.setBackground(color);
	}
	
	// Set color for all labels in line_label, color needs to be 11 long.  
	public static void setLineBackground(Color[] color){
		for(int i = 0; i < 11; i++)
			line_labels[i].setBackground(color[i]);
	}
	
	// Add element to list as a String
	public static void addToList(listEnum list, String value){
		lists[list.ordinal()].add(0, value);
	}

	// Create the application.
	public Grafik() {
		initialize();
		init();
	}
	
	//initialize some other stuff
	private void init(){
		lists = new DefaultListModel[listEnum.values().length];
		for(int i = 0; i < listEnum.values().length; i++){
			lists[i] = new DefaultListModel();
			lists[i].addElement("");
		}
	}
	
	private static String getCurrentTime(){
		long millis = System.currentTimeMillis();
		return String.format("%02d:%02d:%02d,%03d: ", 
					TimeUnit.MILLISECONDS.toHours(millis)%24 + 1, // +1 f�r ? men den l�g en timme fel kanske k�r gmt?
					TimeUnit.MILLISECONDS.toMinutes(millis)%60, 
					TimeUnit.MILLISECONDS.toSeconds(millis)%60, 
					millis%100);
	}
	
	// Initialize the contents of the frame.
	private void initialize() {
		frame = new JFrame();
		frame.addWindowListener(new WindowAdapter(){
			  public void windowClosing(WindowEvent e) {
				    	com.disconnect();
				    }
			  });
		frame.setBounds(100, 100, 867, 721);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(null);
		
		sensor_front_btn = new Button("");
		sensor_front_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setJListVisible(listEnum.frontDistance);
			}
		});
		sensor_front_btn.setBounds(489, 57, 70, 22);
		frame.getContentPane().add(sensor_front_btn);
		
		wheel_right_btn = new Button("");
		wheel_right_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setJListVisible(listEnum.rightWheel);
			}
		});
		wheel_right_btn.setBounds(706, 239, 70, 22);
		frame.getContentPane().add(wheel_right_btn);
		
		sensor_left_btn = new Button("");
		sensor_left_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setJListVisible(listEnum.leftDistance);
			}
		});
		sensor_left_btn.setBounds(240, 158, 70, 22);
		frame.getContentPane().add(sensor_left_btn);
		
		wheel_left_btn = new Button("");
		wheel_left_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setJListVisible(listEnum.leftWheel);
			}
		});
		wheel_left_btn.setBounds(280, 239, 70, 22);
		frame.getContentPane().add(wheel_left_btn);
		
		sensor_right_btn = new Button("");
		sensor_right_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setJListVisible(listEnum.rightDistance);
			}
		});
		sensor_right_btn.setBounds(745, 158, 70, 22);
		frame.getContentPane().add(sensor_right_btn);
		
		turn_left_btn = new JButton("<");
		turn_left_btn.setBounds(35, 522, 64, 55);
		turn_left_btn.addMouseListener(new MouseListener() {
			@Override 
			public void mousePressed(MouseEvent e) {
				sendDirection(directions.turnLeft);
			}

			@Override
			public void mouseClicked(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseEntered(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseExited(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseReleased(MouseEvent arg0) {
				sendDirection(directions.StandStill);
			}
		});
		frame.getContentPane().add(turn_left_btn);
		
		go_back_btn = new JButton("v");
		go_back_btn.setBounds(104, 522, 64, 55);
		go_back_btn.addMouseListener(new MouseListener() {
			@Override 
			public void mousePressed(MouseEvent e) {
				sendDirection(directions.goBackwards);
			}

			@Override
			public void mouseClicked(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseEntered(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseExited(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseReleased(MouseEvent arg0) {
				sendDirection(directions.StandStill);
			}
		});
		frame.getContentPane().add(go_back_btn);
		
		turn_right_btn = new JButton(">");
		turn_right_btn.setBounds(173, 522, 64, 55);
		turn_right_btn.addMouseListener(new MouseListener() {
			@Override 
			public void mousePressed(MouseEvent e) {
				sendDirection(directions.turnRight);
			}

			@Override
			public void mouseClicked(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseEntered(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseExited(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseReleased(MouseEvent arg0) {
				sendDirection(directions.StandStill);
			}
		});
		frame.getContentPane().add(turn_right_btn);
		
		go_forward_btn = new JButton("^");
		go_forward_btn.setBounds(104, 462, 64, 55);
		go_forward_btn.addMouseListener(new MouseListener() {
			@Override 
			public void mousePressed(MouseEvent e) {
				sendDirection(directions.goForwards);
			}

			@Override
			public void mouseClicked(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseEntered(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseExited(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseReleased(MouseEvent arg0) {
				sendDirection(directions.StandStill);
			}
		});
		frame.getContentPane().add(go_forward_btn);
		
		go_left_btn = new JButton("<^");
		go_left_btn.setBounds(35, 462, 64, 55);
		go_left_btn.addMouseListener(new MouseListener() {
			@Override 
			public void mousePressed(MouseEvent e) {
				sendDirection(directions.goLeft);
			}

			@Override
			public void mouseClicked(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseEntered(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseExited(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseReleased(MouseEvent arg0) {
				sendDirection(directions.StandStill);
			}
		});
		frame.getContentPane().add(go_left_btn);
		
		
		go_right_btn = new JButton("^>");
		go_right_btn.setBounds(173, 462, 64, 55);
		go_right_btn.addMouseListener(new MouseListener() {
			@Override 
			public void mousePressed(MouseEvent e) {
				sendDirection(directions.goRight);
			}

			@Override
			public void mouseClicked(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseEntered(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseExited(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseReleased(MouseEvent arg0) {
				sendDirection(directions.StandStill);
			}
		});
		frame.getContentPane().add(go_right_btn);
		
		graficList = new JList();
		listScroll = new JScrollPane(graficList);
		listScroll.setBounds(20, 10, 214, 441);
		listScroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
		frame.add(listScroll);
		
		line_btn = new Button("LINJE");
		line_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setJListVisible(listEnum.line);
			}
		});
		line_btn.setBounds(489, 30, 70, 22);
		frame.getContentPane().add(line_btn);
		
		rgb_btn = new Button("RGB");
		rgb_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setJListVisible(listEnum.rgb);
			}
		});
		rgb_btn.setBounds(350, 104, 70, 22);
		frame.getContentPane().add(rgb_btn);
		
		gyro_btn = new Button("GYRO");
		gyro_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setJListVisible(listEnum.gyro);
			}
		});
		gyro_btn.setBounds(489, 315, 70, 22);
		frame.getContentPane().add(gyro_btn);
		
		line_labels = new JLabel[11];
		for(int i = 0; i < 11; i++){
			line_labels[i] = new JLabel("");
			line_labels[i].setBounds(325 + i * 37, 5, 36, 25);
			line_labels[i].setBackground(Color.black);
			line_labels[i].setOpaque(true);
			frame.getContentPane().add(line_labels[i]);	
		}
		
		battery_label = new JLabel("");
		battery_label.setBounds(780, 5, 36, 25);
		battery_label.setBackground(Color.green);
		battery_label.setOpaque(true);
		frame.getContentPane().add(battery_label);
		
		battery_text_label = new JLabel("Batteri");
		battery_text_label.setBounds(780, 30, 50, 25);
		frame.getContentPane().add(battery_text_label);
		
		rgb_label = new JLabel();
		rgb_label.setBounds(350, 126, 70, 22);
		rgb_label.setBackground(Color.darkGray);
		rgb_label.setOpaque(true);
		frame.getContentPane().add(rgb_label);
		
		Button com_btn = new Button("Set COMport");
		com_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String port = (String)graficList.getSelectedValue();
				if((graficList.getModel() == comPorts) && port != null ){
					com.setup(port, baudRate);
					System.out.println("Port setup f�rdig");
				}
			}
		});
		com_btn.setBounds(484, 600, 80, 22);
		frame.getContentPane().add(com_btn);
		
		Button garbage_btn = new Button("GarbageData");
		garbage_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setJListVisible(listEnum.garbage);
			}
		});
		garbage_btn.setBounds(750, 600, 80, 22);
		frame.getContentPane().add(garbage_btn);
		
		Button refresh_com_btn = new Button("refresh list of COMport");
		refresh_com_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				comPorts = new DefaultListModel();
				String[] ports = com.availablePorts();
				for(int i = 0; i < ports.length; i++){
					comPorts.addElement(ports[i]);
				}
				graficList.setModel(comPorts);
			}
		});
		refresh_com_btn.setBounds(430, 575, 200, 22);
		frame.getContentPane().add(refresh_com_btn);
		
		image_label = new JLabel("");
		image_label.setIcon(new ImageIcon("img/robot.png"));
		image_label.setBounds(300, 10, 456, 567);
		frame.getContentPane().add(image_label);
		
		frame.addMouseListener(new CustomMouseListener());	
	}
	class CustomMouseListener implements MouseListener{
		@Override
		public void mouseClicked(MouseEvent e) {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void mouseEntered(MouseEvent e) {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void mouseExited(MouseEvent e) {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void mousePressed(MouseEvent e) {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void mouseReleased(MouseEvent e) {
			// TODO Auto-generated method stub
			if(is_button_pressed){
				sendDirection(directions.StandStill);
				is_button_pressed = false;
			}
		}
   }
}

