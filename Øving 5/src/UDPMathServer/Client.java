package UDPMathServer;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.Socket;
import java.util.Scanner;

public class Client {


    public static void main(String args[]) throws Exception {

        Scanner scanner = new Scanner(System.in);

        System.out.println("Enter math operation (only ADD and SUBTRACT currently supported)");
        String operation = scanner.nextLine();

        System.out.println("Enter first number");
        String firstNumber = scanner.nextLine();

        System.out.println("Enter second number");
        String secondNumber = scanner.nextLine();

        DatagramSocket clientSocket = new DatagramSocket();
        InetAddress IPAddress = InetAddress.getByName("localhost");

        byte[] transmissionData;
        byte[] receiveData = new byte[1024];

        transmissionData = (operation + " " + firstNumber + " " + secondNumber).trim().getBytes();

        DatagramPacket transmissionPacket = new DatagramPacket(transmissionData, transmissionData.length, IPAddress, 4000);
        clientSocket.send(transmissionPacket);

        DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
        clientSocket.receive(receivePacket);

        String modifiedSentence = new String(receivePacket.getData());
        System.out.println("Response from server:\n" + modifiedSentence);
        clientSocket.close();
    }
}