/*
* CMAC
* (C) 1999-2007,2014 Jack Lloyd
*
* Botan is released under the Simplified BSD License (see license.txt)
*/

#include <botan/cmac.h>
#include <botan/internal/poly_dbl.h>

namespace Botan {

/*
* Perform CMAC's multiplication in GF(2^n)
*/
secure_vector<uint8_t> CMAC::poly_double(const secure_vector<uint8_t>& in)
   {
   secure_vector<uint8_t> out(in.size());
   poly_double_n(out.data(), in.data(), out.size());
   return out;
   }

/*
* Update an CMAC Calculation
*/
void CMAC::add_data(const uint8_t input[], size_t length)
   {
   buffer_insert(m_buffer, m_position, input, length);
   if(m_position + length > output_length())
      {
      xor_buf(m_state, m_buffer, output_length());
      m_cipher->encrypt(m_state);
      input += (output_length() - m_position);
      length -= (output_length() - m_position);
      while(length > output_length())
         {
         xor_buf(m_state, input, output_length());
         m_cipher->encrypt(m_state);
         input += output_length();
         length -= output_length();
         }
      copy_mem(m_buffer.data(), input, length);
      m_position = 0;
      }
   m_position += length;
   }

/*
* Finalize an CMAC Calculation
*/
void CMAC::final_result(uint8_t mac[])
   {
   xor_buf(m_state, m_buffer, m_position);

   if(m_position == output_length())
      {
      xor_buf(m_state, m_B, output_length());
      }
   else
      {
      m_state[m_position] ^= 0x80;
      xor_buf(m_state, m_P, output_length());
      }

   m_cipher->encrypt(m_state);

   for(size_t i = 0; i != output_length(); ++i)
      mac[i] = m_state[i];

   zeroise(m_state);
   zeroise(m_buffer);
   m_position = 0;
   }

/*
* CMAC Key Schedule
*/
void CMAC::key_schedule(const uint8_t key[], size_t length)
   {
   clear();
   m_cipher->set_key(key, length);
   m_cipher->encrypt(m_B);
   poly_double_n(m_B.data(), m_B.size());
   poly_double_n(m_P.data(), m_B.data(), m_P.size());
   }

/*
* Clear memory of sensitive data
*/
void CMAC::clear()
   {
   m_cipher->clear();
   zeroise(m_state);
   zeroise(m_buffer);
   zeroise(m_B);
   zeroise(m_P);
   m_position = 0;
   }

/*
* Return the name of this type
*/
std::string CMAC::name() const
   {
   return "CMAC(" + m_cipher->name() + ")";
   }

/*
* Return a clone of this object
*/
MessageAuthenticationCode* CMAC::clone() const
   {
   return new CMAC(m_cipher->clone());
   }

/*
* CMAC Constructor
*/
CMAC::CMAC(BlockCipher* cipher) : m_cipher(cipher)
   {
   if(m_cipher->block_size() !=  8 && m_cipher->block_size() != 16 &&
      m_cipher->block_size() != 32 && m_cipher->block_size() != 64)
      {
      throw Invalid_Argument("CMAC cannot use the " +
                             std::to_string(m_cipher->block_size() * 8) +
                             " bit cipher " + m_cipher->name());
      }

   m_state.resize(output_length());
   m_buffer.resize(output_length());
   m_B.resize(output_length());
   m_P.resize(output_length());
   m_position = 0;
   }

}
