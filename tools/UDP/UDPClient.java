/******************************************************************************
 * Copyright 2011, Anthony Gelibert <anthony.gelibert@me.com>                 *
 *                                                                            *
 *    Licensed under the Apache License, Version 2.0 (the "License");         *
 *    you may not use this file except in compliance with the License.        *
 *    You may obtain a copy of the License at                                 *
 *                                                                            *
 *        http://www.apache.org/licenses/LICENSE-2.0                          *
 *                                                                            *
 *    Unless required by applicable law or agreed to in writing, software     *
 *    distributed under the License is distributed on an "AS IS" BASIS,       *
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
 *    See the License for the specific language governing permissions and     *
 *    limitations under the License.                                          *
 ******************************************************************************/

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Scanner;

public class UDPClient
{
    private static final String HELP_MESSAGE = "Usage: UDPClient.class @IP port\n";
    private static final String PROMPT = "text>";
    private static final String EXIT_CMD = "exit";

    private UDPClient(){}

    public static void main(final String[] args) throws IOException
    {
        if (args.length != 2)
        {
            System.err.println(HELP_MESSAGE);
            return;
        }
        /* Server IP. */
        final InetAddress address = InetAddress.getByName(args[0]);
        /* Server port. */
        final int port = Integer.parseInt(args[1]);
        /* The packets. */
        final DatagramPacket packet = new DatagramPacket(new byte[1], 1, address, port);
        /* Input. */
        final Scanner input = new Scanner(System.in);
        /* UDP socket. */
        final DatagramSocket socket = new DatagramSocket();
        while (true)
        {
            System.out.print(PROMPT);
            final String line = input.nextLine();
            if (EXIT_CMD.compareTo(line) == 0)
            {
                break;
            }
            packet.setData(line.getBytes());
            packet.setLength(line.getBytes().length);
            socket.send(packet);
        }
        socket.close();
    }
}
