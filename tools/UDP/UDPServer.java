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

/**
 * Simple UDP Client.
 *
 * @author LCIS/CTSYS - Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 * @version March 21, 2011
 */
public class UDPServer
{
    /**
     * UDP Buffer size.
     */
    private static final int BUFFER_SIZE = 20;
    /**
     * UDP Buffer.
     */
    private static final byte[] BUFFER = new byte[BUFFER_SIZE];

    private static final String HELP_MESSAGE = "Usage: UDPServer.class port\n";

    private UDPServer(){}

    public static void main(final String[] args) throws IOException
    {
        if (args.length != 1)
        {
            System.err.println(HELP_MESSAGE);
            return;
        }

        /* UDP socket. */
        final DatagramPacket data = new DatagramPacket(BUFFER, BUFFER_SIZE);
        final DatagramSocket socket = new DatagramSocket(Integer.parseInt(args[0]),
                                                         InetAddress.getByName("192.168.1.1"));
        System.out.println("I listen on "+ socket.getLocalAddress().toString() +
                           ':' + socket.getLocalPort());
        while (true)
        {
            socket.receive(data);
            System.out.println("I received '" + new String(data.getData(),0,data.getLength()) +
                               "' from '" + data.getAddress().toString() +
                               "' on port '" + data.getPort() + '\'');

        }
    }
}
