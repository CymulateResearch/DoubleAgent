# Double Agent

## Overview

This project demonstrates a vulnerability in Azure Active Directory (AAD) when syncing multiple on-premises AD domains to a single Azure tenant. The vulnerability arises when authentication requests are mishandled by Pass-Through Authentication (PTA) agents for different on-prem domains, potentially leading to unauthorized access.

**Note:** This project is for educational and research purposes only. Do not use this in production environments or for malicious purposes.

## Background

Azure Active Directory Pass-Through Authentication (PTA) allows users to sign in to both on-premises and cloud-based applications using the same passwords. However, our research has uncovered a flaw in the credential validation process that could allow attackers to bypass security checks and gain unauthorized access.


## Technical Details

The project consists of two main components:

1. A managed DLL that hooks the `ValidateCredentials` method using the Harmony library
2. An unmanaged C++ DLL that loads the managed DLL into the PTA agent process

## Usage
1. Compile both the managed (C#) and unmanaged (C++) projects.

2. For the C# project:
   - Add the dll inside the folder as a reference to the project.

## References
- [Original blog post link]
