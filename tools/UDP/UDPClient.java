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
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Scanner;

/**
 * Simple UDP Client.
 *
 * @author LCIS/CTSYS - Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 * @version March 21, 2011
 */
public class UDPClient
{
    /** Help message. */
    private static final String HELP_MESSAGE = "Usage: UDPClient.class @IP port\n";
    /** Prompt. */
    private static final String PROMPT       = "text>";
    /** Command to "exit". */
    private static final String EXIT_CMD     = "exit";

    private UDPClient() {}


    /**
     * Main method.
     *
     * @param args Wait for 2 arguments: "@IP", "port"
     *
     * @throws UnknownHostException Unknown DNS name
     */
    public static void main(final String[] args) throws UnknownHostException
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
        try
        {
            final DatagramSocket socket = new DatagramSocket();
            try
            {
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
