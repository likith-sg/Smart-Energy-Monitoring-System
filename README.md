# Smart Home Energy Monitoring System

This repository contains the code for a smart home energy monitoring system. The system is designed to track the usage of individual appliances in real-time with an accuracy of 89%.

## Contents

The repository is structured as follows:

- `ct_apr29a/`
  - `ct_apr29a.ino`: Main Arduino sketch for the current transformer (CT) based monitoring system.
  - `EmonLib.h`: Library for energy monitoring.
  - `emonct_may14a/`
    - `emonct_may14a.ino`: Arduino sketch for an updated version of the CT-based monitoring system.
    - `ct_may14a/`
      - `ct_may14a.ino`: Further refinement of the CT-based monitoring system.
- `Main_may13a/`
  - `Main_may13a.ino`: Main Arduino sketch for the overall system.
- `Switch_apr22a/`
  - `Switch_apr22a.ino`: Code for controlling switches in the system.

## Description

### ct_apr29a

This folder contains the initial implementation of the current transformer (CT) based energy monitoring system. The `ct_apr29a.ino` file is the main Arduino sketch, and it uses the `EmonLib.h` library for energy monitoring.

### emonct_may14a

This subfolder within `ct_apr29a` contains an updated version of the CT-based monitoring system. The `emonct_may14a.ino` file includes improvements over the initial implementation.

### ct_may14a

Nested within `emonct_may14a`, this folder includes further refinements to the CT-based monitoring system. The `ct_may14a.ino` file is the Arduino sketch for this version.

### Main_may13a

The `Main_may13a` folder contains the main Arduino sketch for the overall energy monitoring system. This file integrates various components of the system.

### Switch_apr22a

The `Switch_apr22a` folder contains the code for controlling switches in the energy monitoring system. The `Switch_apr22a.ino` file is the Arduino sketch responsible for switch operations.

## Usage

1. **Hardware Setup**: Connect the current transformers (CTs) to the appliances you want to monitor. Ensure that the Arduino and other components are properly connected as per the circuit diagram.
2. **Upload Code**: Open the Arduino IDE and upload the respective `.ino` files to the Arduino board. Start with the `Main_may13a.ino` for the overall system, and then upload the specific CT-based monitoring sketches if necessary.
3. **Monitor Energy Usage**: Once the code is uploaded, the system will start monitoring the energy usage of the connected appliances. You can view the real-time usage data through the Arduino IDE's Serial Monitor or any connected display/interface.

## Accuracy

The system has been tested and achieved an accuracy of 89% in tracking the usage of individual appliances. This makes it a reliable solution for real-time energy monitoring in smart homes.

## Additional Information

For a better view and more details, check out this [LinkedIn post](https://www.linkedin.com/posts/nishaan-gowda-s-r-6b7b07299_smart-plug-innovation-thrilled-to-ugcPost-7197272731984277504-3Kf3?utm_source=share&utm_medium=member_desktop). You can also contact me through my [GitHub profile](https://github.com/likith-sg) for further documentation or inquiries.

## License

This project is licensed under the MIT License.

## Acknowledgements

- [EmonLib](https://github.com/openenergymonitor/EmonLib) - The energy monitoring library used in this project.
