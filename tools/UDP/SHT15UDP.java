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

public class SHT15UDP
{

    private static final int BUFFER_SIZE = 10;
    private static final byte[] BUFFER = new byte[BUFFER_SIZE];

    private SHT15UDP()
    {
    }

    public static void main(final String[] args) throws IOException
    {
        System.out.println("Hello !!!");
        /* UDP socket. */
        final DatagramPacket data = new DatagramPacket(BUFFER, BUFFER_SIZE);
        final DatagramSocket socket = new DatagramSocket(1234, InetAddress.getByName("172.28.1.224"));
        System.out.println("I wait tmp/hr data on UDP port 1234\n");
        while (true)
        {
            socket.receive(data);
            final String[] values = new String(data.getData()).split("-", 2);
            final double tmp = -39.7 + 0.01 * Double.parseDouble(values[0]);
            System.out.println("Current TMP is : " + tmp);
            final int hrValue = Integer.parseInt(values[1]);
            //final double hr =
            System.out.println("Current HR is : "  );
        }
    }
}
