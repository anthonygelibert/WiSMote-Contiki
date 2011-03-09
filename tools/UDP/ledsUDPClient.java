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

public class LEDsUDPClient
{
    private static final String HELP_MESSAGE = "Usage: UDPClient.class @IP port\n";
    private static final String PROMPT = "text>";
    private static final String BLUE_CMD = "blue";
    private static final String GREEN_CMD = "green";
    private static final String RED_CMD = "red";
    private static final String EXIT_CMD = "exit";
    private static final String BLUE_LED_PACKET = "LED-1";
    private static final String GREEN_LED_PACKET = "LED-2";
    private static final String RED_LED_PACKET = "LED-3";

    private LEDsUDPClient()
    {
    }

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
            String line = input.nextLine();
            if (BLUE_CMD.compareTo(line) == 0)
            {
                line = BLUE_LED_PACKET;
            }
            else
            {
                if (GREEN_CMD.compareTo(line) == 0)
                {
                    line = GREEN_LED_PACKET;
                }
                else
                {
                    if (RED_CMD.compareTo(line) == 0)
                    {
                        line = RED_LED_PACKET;
                    }
                    else
                    {
                        if (EXIT_CMD.compareTo(line) == 0)
                        {
                            break;
                        }
                    }
                }
            }
            packet.setData(line.getBytes());
            packet.setLength(line.getBytes().length);
            socket.send(packet);
        }
        socket.close();
    }
}
