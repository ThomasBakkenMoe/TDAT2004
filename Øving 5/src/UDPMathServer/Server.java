package UDPMathServer;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class Server {

        enum mathOperations{
            ADD,
            SUBTRACT
        }

        private static double handleOperation  (String operator, double operand1, double operand2) throws IllegalArgumentException {

            double result = 0;
            System.out.println("input operator: " + operator.toUpperCase());
            switch (mathOperations.valueOf(operator.toUpperCase())){
                case ADD:
                    System.out.println("Addition");
                    result = operand1 + operand2;
                    System.out.println(result);
                    return result;
                case SUBTRACT:
                    System.out.println("Subtraction");
                    result = operand1 - operand2;
                    System.out.println(result);
                    return result;
                default:
                    throw new IllegalArgumentException("Ugyldig operator");
            }
        }

    public static void main(String[] args) throws IOException {

        //Initialize server/API
        final int PORT_NR = 4000;
        DatagramSocket serverSocket = new DatagramSocket(PORT_NR); // Java UDP socket

        byte[] transmissionData = new byte[1024];
        byte[] receiveData = new byte[1024];

        DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
        DatagramPacket transmissionPacket;

        System.out.println("Server running! Port number: " + PORT_NR);

        //It would probably still be smart to have a way of neatly shutting off the server
        while (true){
            serverSocket.receive(receivePacket);

            String receiveString =  new String(receivePacket.getData()); // Convert incoming byte array to string
            System.out.println("Incoming data: " + receiveString);

            String[] inputParameters = receiveString.split(" ");
            System.out.println(inputParameters[0]);

            try {
                transmissionData = String.valueOf(handleOperation(inputParameters[0], Double.parseDouble(inputParameters[1]), Double.parseDouble(inputParameters[2]))).getBytes();
                transmissionPacket = new DatagramPacket(transmissionData, transmissionData.length, receivePacket.getAddress(), receivePacket.getPort());
                serverSocket.send(transmissionPacket);

                //Clear buffers for next run-through
                transmissionData = new byte[1024];
                receiveData = new byte[1024];
                receiveString = null;

            }catch (IllegalArgumentException e){

                //Clear buffers for next run-through
                transmissionData = new byte[1024];
                receiveData = new byte[1024];
                receiveString = null;
                e.printStackTrace();
            }
        }
    }
}
