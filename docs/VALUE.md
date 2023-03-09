# Value of CrossCryptFS

This document aims to explain the value of CrossCryptFS from a non technical
standpoint.

## Cross platform (Works on any hardware, any operating system)

Many disk encryption mechanisms require that a variety of hardware and operating systems all support the same mechanism. This is very rare in practice with particular approaches being supported by only some operating systems. By implementing a file system on top of any existing mounted disk and raw file system, you have flexibility.

CrossCryptFS will also work on top of any type of disc (USB stick, SSD, or large spinning magnetic HDD) and any Operating System (Mac OS, Linux, Windows, mobile operating systems, etc.). It's also simple enough to be implemented in very small hardware too, such as encrypting data on flash chips within embedded devices. (E.g. on Wearables)

## User Accessible (No Admin rights needed)

Most full disk encryption mechanisms need technical skill across multiple platforms to implement - beyond most users and requiring expert support. They may also need administrator level permissions to install the requisite device drivers to use. CrossCryptFS provides a user-executable Command Line Interface (CLI) making it easier to use.

## Error checking or deliberate data modification detection

If you need full disk encryption you can employ both full disk encryption and file system encryption. File System Encryption has additional benefits because most disk encryption software doesn't have error checking (or 'authentication tags' in the parlance) to verify that data has not been manipulated or corrupted on disc. With gene sequence data where a single hardware fault on a single bit could lead to misclassification this is a useful facility to have.

## Keeping up to date with the latest encryption software and settings

Security problems are being found all the time, and computing power is always improving. Every year some existing encryption mechanism becomes deemed 'no longer secure'. By providing an additional software based File System Encryption mechanism like CrossCryptFS that is pluggable with different encryption mechanisms, you can stay ahead of the latest advances in encryption.

## More secure transport

Most full disk encryption mechanisms use symmetric key encryption. This means that the full key needs to be transported from the data sender to the data recipient via some unspecified secure mechanism. This is often the weak link.

With CrossCryptFS there will be mechanisms that use public key cryptography - allowing the recipient to share their public key before data is encrypted, and allowing the sender to encrypt data and send it without sharing any 'symmetric key', or revealing any data that would allow the symmetric key to be discovered. This is more secure and means the encrypted data and senders public key can be transmitted without fear of interception. (Although, of course, you should still try to prevent data interception!)

## Other benefits

There are other benefits but these tend to be of a technical nature. Please see the [Layers](LAYERS.md) document for more details on these, or the higher level [Design](DESIGN.md) document.