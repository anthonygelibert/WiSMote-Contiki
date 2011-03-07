import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Scanner;

public class ledsUDPClient
{
    public static void main(String[] args) throws SocketException, UnknownHostException, IOException
    {
        if (args.length != 2)
        {
            System.err.println("Usage: UDPClient.class @IP port\n");
            return;
        }
        /* Server port. */
        final int port = Integer.parseInt(args[1]);
        /* Server IP. */
        final String ip = args[0];
        /* UDP socket. */
        final DatagramSocket socket = new DatagramSocket();
        /* Input. */
        final Scanner input = new Scanner(System.in);
        String line;
        do
        {
            System.out.print("text>");
            line = input.nextLine();
            if ("blue".compareTo(line) == 0)
            {
                socket.send(new DatagramPacket("LED-1\n".getBytes(), "LED-1\n".length(), InetAddress.getByName(ip), port));
                continue;
            }
            if ("green".compareTo(line) == 0)
            {
                socket.send(new DatagramPacket("LED-2\n".getBytes(), "LED-2\n".length(), InetAddress.getByName(ip), port));
                continue;
            }
            if ("red".compareTo(line) == 0)
            {
                socket.send(new DatagramPacket("LED-3\n".getBytes(), "LED-3\n".length(), InetAddress.getByName(ip), port));
                continue;
            }
            socket.send(new DatagramPacket(line.getBytes(), line.length(), InetAddress.getByName(ip), port));
        }
        while ("exit".compareTo(line) != 0);
        System.out.println("Good Bye !!!");
    }
}
