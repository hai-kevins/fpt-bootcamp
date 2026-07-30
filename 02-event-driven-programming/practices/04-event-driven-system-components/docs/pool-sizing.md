# Event Pool Sizing

Sizing starts from the maximum simultaneous outstanding dynamic events. Track:

- current used blocks;
- high-water mark;
- allocation failures;
- invalid releases;
- maximum fan-out from publish-subscribe.

The root pool uses 32 fixed-size event blocks for study.
