/*
 * Copyright (c) 2008-2014 John Connor (BM-NC49AxAjcqVcF5jNPu85Rb8MJ2d9JqZt)
 *
 * This file is part of coinpp.
 *
 * coinpp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DATABASE_NAT_PMP_CLIENT_HPP
#define DATABASE_NAT_PMP_CLIENT_HPP

#include <deque>
#include <functional>
#include <vector>

#include <boost/asio.hpp>

#include <database/nat_pmp.hpp>

namespace database {

    /**
     * Implements the nat-pmp protocol.
     */
    class nat_pmp_client
    {
        public:
        
            /**
             * Constructor
             * @param ios The boost::asio::io_service object to use.
             */
            explicit nat_pmp_client(boost::asio::io_service & ios);
            
            /**
             * Start the nat-pmp client.
             */
            void start();
            
            /**
             * Stops the nat-pmp client removing all mappings.
			 * @param unmap
             */
            void stop(const bool & unmap = true);
        
            /**
             * Adds a mapping.
             * @param protocol The protocol.
             * @param port The port.
             */
            void add_mapping(
                const nat_pmp::protocol_t & protocol, const std::uint16_t & port
            );
        
            /**
             * Removes a mapping.
             * @param protocol The protocol.
             * @param port The port.
             */
            void remove_mapping(
                const nat_pmp::protocol_t & protocol, const std::uint16_t & port
            );
        
            /**
             * Set the port map success callback.
             * @param func
             */
            void set_map_port_success_callback(
                const std::function<void (std::uint16_t protocol,
                std::uint16_t private_port, std::uint16_t public_port)> & 
            );
            
            /**
             * Sends a mapping request with the given protocol, private port,
             * public port and lifetime.
             * @param protocol
             * @param private_port
             * @param public_port
             * @param lifetime
             * @param queue
             */
            void send_mapping_request(
                std::uint16_t protocol, std::uint16_t private_port, 
                std::uint16_t public_port, std::uint32_t lifetime
            );
            
            /**
             * Runs the test case.
             */
            static int run_test();
            
        private:
            
            /**
             * Sends a public address request to the gateway.
             */
            void send_public_address_request();
            
            /**
             * Performs a public address request re-transmission.
             */
            void retransmit_public_adddress_request(
                const boost::system::error_code & ec
            );
            
            /**
             * Sends a request to the gateway.
             */
            void send_request(nat_pmp::mapping_request & req);
            
            /**
             * Sends any queued requests.
             */
            void send_queued_requests();
            
            /**
             * Sends buf of size len to the gateway.
             */
            void send(const char * buf, std::size_t len);
            
            /**
             * Send handler.
             */
            void handle_send(
                const boost::system::error_code & ec, std::size_t bytes
            );
            
            /**
             * Asynchronous cannot handler.
             */
            void handle_connect(const boost::system::error_code & ec);
			
			/**
			 * The timeout handler.
			 * @param ec
			 */
			void handle_timeout(const boost::system::error_code &);
			
            /**
             * Asynchronous receive from handler.
             */
            void handle_receive_from(
                const boost::system::error_code & ec, std::size_t bytes
            );
            
            /**
             * Asynchronous response handler.
             */
            void handle_response(const char * buf, std::size_t len);

            /**
             * The mapping refresh handler.
             * @param ec
             */
            void tick(const boost::system::error_code &);
            
            /**
             * The ip address of the gateway.
             */
            boost::asio::ip::address m_gateway_address;
            
            /**
             * The ip address on thw WAN side of the gateway.
             */
            boost::asio::ip::address m_public_ip_address;
            
        protected:
        
            /**
             * The boost::asio::io_service.
             */
            boost::asio::io_service & io_service_;
        
            /**
             * The boost::asio::strand.
             */
            boost::asio::strand strand_;
            
            /**
             * The connection timeout timer.
             */
            boost::asio::deadline_timer timeout_timer_;
            
            /**
             * The request retry timer.
             */
            boost::asio::deadline_timer retry_timer_;
            
			/**
			 * The mapping refresh timer.
			 */
			boost::asio::deadline_timer refresh_timer_;
                        
            /**
             * The udp socket.
             */
            std::unique_ptr<boost::asio::ip::udp::socket> socket_;
            
            /**
             * The gateway endpoint.
             */
            boost::asio::ip::udp::endpoint endpoint_;
            
            /**
             * The non-parallel public ip address request.
             */
            nat_pmp::mapping_request public_ip_request_;
            
            /**
             * The parallel reuqest queue.
             */
            std::deque<nat_pmp::mapping_request> request_queue_;
            
            /**
             * The receive buffer length.
             */
            enum
            {
                receive_buffer_length = 512
            };
            
            /**
             * The receive buffer.
             */
            char data_[receive_buffer_length];
        
            /**
             * Mappings that we are responsible for.
             */
            std::vector<
                std::pair<nat_pmp::mapping_request, nat_pmp::mapping_response>
            > mappings_;
			
            /**
             * Map port success callback.
             */
            std::function<
            	void (std::uint16_t protocol, std::uint16_t private_port,
            	std::uint16_t public_port)
           	> nat_pmp_map_port_success_cb_;
    };
    
} // namespace database

#endif // DATABASE_NAT_PMP_CLIENT_HPP
