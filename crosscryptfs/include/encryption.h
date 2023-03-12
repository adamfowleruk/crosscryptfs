//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <iosfwd>
#include <string>

namespace crosscryptfs {

/*!
 * \brief High level virtual interface class with common functions required of all EncryptionProviders
 */
class EncryptionProvider {
public:
    EncryptionProvider() = default;
    virtual ~EncryptionProvider() = default;
    EncryptionProvider(const EncryptionProvider&) = delete;
    EncryptionProvider(EncryptionProvider&&) = delete;
    EncryptionProvider& operator=(const EncryptionProvider&) = delete;
    EncryptionProvider& operator=(EncryptionProvider&&) = delete;

    // virtual std::string getShortPrefix() = 0;
    // virtual std::string getLongName() = 0;
    // virtual std::string getShortDescription() = 0;
    
    /*!
     * \brief Sets a parameter for this EncryptionProvider. Normally read by CrossCryptFS from managed volume settings file(s)
     */
    virtual void setParameterValue(std::string name,std::string value) = 0;
    // virtual void setParameterFile(std::string fileExtension,std::string value) = 0;
    /*!
     * \brief Return all parameter values that should be saved. Normally written by CrossCryptFS to managed volume settings file(s)
     */
    virtual std::string getParameterValues() = 0;

    /*!
     * \brief Sets the (Perhaps separately encrypted) 'keyfile' contents for this EncryptionProvider.
     *
     * May be a simple binary for a symmetric key, or a more complex format.
     */
    virtual void setKeyMaterial(std::istream& keyDataIn) = 0;

    // Note that these methods assume the resulting encrypted text also includes any data (aside from settings) required for decryption
    /*!
     * \brief Encrypts the provided plaintext content and appends the ciphertext to the provided stream
     *
     * May also include EncryptionProvider specific content, such as the Authentication Tag for the file, as in ECIESEncryptionProvider
     */
    virtual void encryptContent(std::istream& plainIn,std::ostream& encryptedOut) = 0;
    /*!
     * \brief Decrypts the provided ciphertext content and appends the plaintext to the provided stream
     */
    virtual void decryptContent(std::istream& encryptedIn,std::ostream& plainOut) = 0;
};





// class IEncryptionBytesProvider: public IBytesProvider {
// public:
//     IEncryptionBytesProvider(EncryptionProvider& source, std::istream& plainIn) noexcept
//         : sourceEncryption(source),
//           plainIn(plainIn),
//           buffer()
//     {
//         ;
//     }
//     ~IEncryptionBytesProvider() noexcept {};
//     IEncryptionBytesProvider(const IEncryptionBytesProvider&) = delete;
//     IEncryptionBytesProvider(IEncryptionBytesProvider&&) = delete;
//     IEncryptionBytesProvider& operator=(const IEncryptionBytesProvider&) = delete;
//     IEncryptionBytesProvider& operator=(IEncryptionBytesProvider&&) = delete;

//     size_t getNextBytes(ByteArray& bytes) noexcept override
//     {
//         plainIn.read(buffer,BYTEARRAYSIZE);
//         //
//     }

// private:
//     EncryptionProvider& sourceEncryption;
//     std::istream& plainIn;
//     ByteArray buffer;
// };

// using EncryptionStreamBytesProvider = std::shared_ptr<IEncryptionBytesProvider>;

} // end namespace

#endif