import java.awt.EventQueue;
import java.awt.Image;

import javax.swing.JFrame;
import javax.swing.JToggleButton;
import java.awt.BorderLayout;

import javax.swing.DefaultListModel;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.SpringLayout;
import javax.swing.JPanel;
import java.awt.Canvas;
import java.awt.Color;

import javax.swing.JList;
import java.awt.Button;
import javax.swing.JLabel;
import org.eclipse.wb.swing.FocusTraversalOnArray;
import java.awt.Component;

enum listEnum{leftDistance, rightDistance, frontDistance, gyro, leftWheel, rightWheel, line, rgb};
enum directions{goForwards, goBackwards, goLeft, goRight, turnLeft, turnRight}; 

public class Grafik {
	// The frame that everything gets drawn into.
	private JFrame frame;
	
	// buttons 4 distance sensors
	private Button sensor_front_btn;
	private Button sensor_left_btn;
	private Button sensor_right_btn;
	
	// button 4 gyro
	private Button gyro_btn;
	
	// buttons 4 wheels
	private Button wheel_right_btn;
	private Button wheel_left_btn;
	
	// button 4 line
	private Button line_btn;
	
	// button 4 rgb
	private Button rgb_btn;
	
	//buttons stearing
	private JButton go_right_btn;
	private JButton go_left_btn;
	private JButton go_forward_btn;
	private JButton turn_right_btn;
	private JButton go_back_btn;
	private JButton turn_left_btn; 
	
	// THE LISTs
	private JList graficList;
	private DefaultListModel[] lists;

	// Labels
	private JLabel line_labels[]; // Used to show a grafical version of the different line sensors 
	private JLabel image_label;	  // Used for the background image
	private JLabel rgb_label;	  // Use to show if the RGB is getting any data  
	
	// Bluetooth device
	private Bluetooth bt = new Bluetooth();
	
	// Launch the application.
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
	}
	
	// label buttons
	public void setButtonLabel(listEnum button, String value){
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

	// Select what data to write to the list
	private void setJListVisible(listEnum list){
		graficList.setModel(lists[list.ordinal()]);
	}
	
	// Set color on rgb_label
	public void setRGBBackground(Color color){
		rgb_label.setBackground(color);
	}
	
	// Set color for all labels in line_label, color needs to be 11 long.  
	public void setLineBackground(Color[] color){
		for(int i = 0; i < 11; i++)
			line_labels[i].setBackground(color[i]);
	}
	
	// Add element to list as a String
	public void addToList(listEnum list, String value){
		lists[list.ordinal()].addElement(value);
	}

	// Create the application.
	public Grafik() {
		initialize();
		init();
	}
	
	//initialize some other stuff
	private void init(){
		lists = new DefaultListModel[8];
		for(int i = 0; i < 8; i++){
			lists[i] = new DefaultListModel();
			lists[i].addElement("asd"+i);
		}
	}
	
	// Initialize the contents of the frame.
	private void initialize() {
		frame = new JFrame();
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
		turn_left_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				bt.sendDirection(directions.turnLeft);
			}
		});
		frame.getContentPane().add(turn_left_btn);
		
		go_back_btn = new JButton("v");
		go_back_btn.setBounds(104, 522, 64, 55);
		go_back_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				bt.sendDirection(directions.goBackwards);
			}
		});
		frame.getContentPane().add(go_back_btn);
		
		turn_right_btn = new JButton(">");
		turn_right_btn.setBounds(173, 522, 64, 55);
		turn_right_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				bt.sendDirection(directions.turnRight);
			}
		});
		frame.getContentPane().add(turn_right_btn);
		
		go_forward_btn = new JButton("^");
		go_forward_btn.setBounds(104, 462, 64, 55);
		go_forward_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				bt.sendDirection(directions.goForwards);
			}
		});
		frame.getContentPane().add(go_forward_btn);
		
		go_left_btn = new JButton("<^");
		go_left_btn.setBounds(35, 462, 64, 55);
		go_left_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				bt.sendDirection(directions.goLeft);
			}
		});
		frame.getContentPane().add(go_left_btn);
		
		
		go_right_btn = new JButton("^>");
		go_right_btn.setBounds(173, 462, 64, 55);
		go_right_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				bt.sendDirection(directions.goRight);
			}
		});
		frame.getContentPane().add(go_right_btn);
		
		graficList = new JList();
		graficList.setBounds(35, 10, 199, 441);
		frame.getContentPane().add(graficList);
		
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
		
		rgb_label = new JLabel();
		rgb_label.setBounds(350, 126, 70, 22);
		rgb_label.setBackground(Color.darkGray);
		rgb_label.setOpaque(true);
		frame.getContentPane().add(rgb_label);
		
		image_label = new JLabel("");
		image_label.setIcon(new ImageIcon("C:\\Users\\Pontus\\workspace\\KMM\\img\\robot.png"));
		//Image img = new ImageIcon(this.getClass().getResource("robot.png")).getImage();
		//image_label.setIcon(new ImageIcon(img));
		image_label.setBounds(300, 10, 456, 567);
		frame.getContentPane().add(image_label);
	}
}
