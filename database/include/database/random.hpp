/*
 * Copyright (c) 2008-2015 John Connor (BM-NC49AxAjcqVcF5jNPu85Rb8MJ2d9JqZt)
 *
 * This is free software: you can redistribute it and/or modify
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

#ifndef DATABASE_RANDOM_HPP
#define DATABASE_RANDOM_HPP

#include <string>

namespace database {

    /**
     * Implements random functionality.
     */
    class random
    {
        public:
        
            static std::string string(const std::size_t & length)
            {
                std::string ret(length, 0);
                
                auto randchar = []() -> char
                {
                    const char charset[] =
                    "0123456789"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz";
                    const std::size_t max_index = (sizeof(charset) - 1);
                    return charset[std::rand() % max_index];
                };

                std::generate_n(ret.begin(), length, randchar);
                
                return ret;
            }
        
        private:
        
            // ...
        
        protected:
        
            // ...
    };

} // namespace database

#endif // DATABASE_RANDOM_HPP
