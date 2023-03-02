# crosscryptfs
Cross platform filesystem with encryption options.

## Motivation

I've often wondered why in 2023 there's not a cross platform filesystem option that works across Mac, Windows and Linux. This hasn't been a major concern for a while - which is why I guess no-one has yet solved it - but it is now becoming irksome.

A colleague in research wanted to transfer some files that included human genome sequence data between institutions. The source system was Windows, she had a mac, and the target systems at the other institution was a compute cluster on Linux. The combination of external hard drive (for transport) with its NTFS file system, the use of an encrypted ZIP file on this drive spanning multiple files, and the target system having a shared network folder structure meant that decrypting the information whilst in transfer was impractical and failed after over 20 hours. In 2023 we must be able to do better, surely?

## Initial research

I had another quick look (as I do for this problem every year or so) and came to the conclusion that the nearest things we have currently are as follows:-

- Bitlocker encrypted discs on Windows
- EncFS in user space in FUSE on Linux
- eCryptFS in kernel space on Linux
- FileVault on Mac
- Hard disc manufacturer tooling, typically Windows only and vendor specific
- Hardware devices with the encryption happening in an embedded device (E.g. protected by a PIN code on the physical USB stick)

Clearly none of the above are really cross platform, but from a user perspective the best and most usable option is the USB stick. I've used these in the past and its possible to get tamper proof units too, useful for government grade security use cases for data in transport.

## Design needs

Whilst the above solve a number of key technical issues, there are user issues that are only partially addressed for specific, but I don't think niche, use cases.

Below are use cases I think should be considered, with the most common at the top:-

- Good file system performance for day to day use, including as a system drive, on all storage media (HDD, SDD, USB sticks, ideally also spanned RAID arrays)
- Use of a disc between different operating systems
- Moving one disc from use on one system to another (E.g. if I buy a new laptop with the same OS, so distinct from the above use case)
- Transferring large amounts of files between systems owned by different people
- Secure transfer (with the key being transferred separate to the encrypted disc via another mechanism)
- Encrypting or decrypting a drive that is already in use with over 50% of disc space already used

## Starting simply

Rather than try to boil the ocean with building YetAnotherFileSystem [tm] I think a more practical approach is to solve the multi-platform issue first with an encryption mode that can be done in user land with command line tools initially, and a file system drive eventually.

Today the most commonly supported cross platform file system is ExFAT which is used typically by USB sticks across Windows, Mac, and Linux. This file system can be
read and written on all these operating systems. Layering a file-level encryption approach on top of this seems the easiest quick win.

For encryption, an approach similar to EncFS but which uses modern encryption standards and where code could take advantage of hardware encryption/decryption instructions seems the quickest approach. To solve my original colleagues issue this approach should have the following additional features:-

- Have the encryption/decryption tooling execute in user land without special privileges
- Be able to encrypt/decrypt existing files/folders either in-situe on the same disc, or to/from a target filesystem folder
- Support network drive storage as a source/destination
- Be resilient to mid-decryption failures such as slow network file system access, and allow for automatic and delayed resumption of an encryption or decryption operation
- Allow the encryption password and/or key to be transferred separately from the data itself
- Have fair performance on common external hard drives for data sizes up to 8TB

(Yeah so maybe the 8TB with decent performance is not 'simple', but it is a key requirement)

I believe the best approach is therefore as follows:-

- Develop a cross platform CLI tool that can take a folder and encrypt it, and do the reverse, using file-level encryption on an existing mounted file system
  - I'll do this initially on an external LaCie rugged SSD pro 2TB disc I have on my mac, formatted for the mac
  - I'll choose an encryption mechanism that is widely supported, such as by OpenSSL 3 (which is Apache 2.0 licensed)
- Test this resultant approach on an ExFAT unencrypted USB stick across different operating systems
  - I have an existing 2 GB exFAT USB stick that is quite old but which I know had good access speeds across multiple systems
  - I also have a Samsung Evo SSD and a USB 3 adapter which I can use for testing, across a Linux Manjaro desktop, Windows 10 desktop, and MacBook Pro (new Max chip)
- Get the two academic institutions to sign off on this and use it for my friends data access needs
- Create a user space file system support for this
  - FUSE driver for Linux
  - Equivalent for Mac and Windows, if supported, or kernel level if not

## License and Copyright

All software files are licensed Apache 2.0 unless otherwise stated. All documentation is licensed CC-BY-4.0. Copyright the crosscryptfs project authors 2023 All Rights Reserved.