/*
 * Copyright (c) 2011, LCIS/CTSYS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

/**
 * Simple UDP Server.
 *
 * @author LCIS/CTSYS - Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 * @version March 23, 2011
 */
public class UDPServer
{
    /** UDP Buffer size. */
    private static final int    BUFFER_SIZE = 500;
    /** UDP Buffer. */
    private static final byte[] BUFFER      = new byte[BUFFER_SIZE];

    private static final String HELP_MESSAGE = "Usage: UDPServer.class port\n";

    private UDPServer() {}

    /**
     * Main method.
     *
     * @param args Wait for 2 arguments: "@IP", "port"
     */
    public static void main(final String[] args)
    {
        if (args.length != 1)
        {
            System.err.println(HELP_MESSAGE);
            return;
        }

        /* UDP socket: data */
        final DatagramPacket data = new DatagramPacket(BUFFER, BUFFER_SIZE);
        /* UDP socket */
        try
        {
            final DatagramSocket socket = new DatagramSocket(Integer.parseInt(args[0]));
            try
            {
                System.out.println("I listen on " + socket.getLocalAddress().toString() +
                                   ':' + socket.getLocalPort());
                while (true)
                {
                    socket.receive(data);
                    System.out.println("I received '" + new String(data.getData(), 0, data.getLength()) +
                                       "' from '" + data.getAddress().toString() +
                                       "' on port '" + data.getPort() + '\'');

                }
            }
            finally
            {
                socket.close();
            }
        }
        catch (final IOException e)
        {
            e.printStackTrace();
        }
    }
}
