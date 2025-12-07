<div align="center">
    <img src="assets/LOGO.png" width="200">
    <h1>PIXPATCH (WIP)</h1>
</div>

**A resilient, end-to-end encrypted dispatch system for hostile network environments.**

*Built for the HackYeah 2025 - Defence.*

---

### Problem

In conflict zones or disaster areas, traditional communication infrastructure is often the first thing to fail. Centralized systems like cell towers and internet exchanges are vulnerable targets, making it impossible for aid workers, journalists, and civilians to communicate securely. Existing decentralized solutions often lack the robust security or versatility needed when messages contain life-or-death information.

### Solution: Pixpatch

Pixpatch is a proof-of-concept terminal application that demonstrates a new paradigm for secure communication. It is a dispatch system built on top of **[EFPIX](https://github.com/shinymonitor/libefpix)**, a zero-trust, encrypted flood protocol.

This approach provides:
*   **Resilience:** Messages are flooded through all available nodes, finding a path around failures.
*   **Security:** State-of-the-art end-to-end encryption (X25519, ChaCha20-Poly1305) ensures only the recipient can read messages.
*   **Anonymity:** The protocol is designed to protect the metadata of who is talking to whom.
*   **Serverless:** It requires no central server, making it impossible to shut down.
