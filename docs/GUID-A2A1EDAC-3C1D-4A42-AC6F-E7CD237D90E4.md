# SRV_PSERIAL_COMMAND Enum

## C

```c
typedef enum
{
  /* Get data configuration request */
  SRV_PSERIAL_CMD_PHY_GET_CFG = 0,
  /* Get data configuration response */
  SRV_PSERIAL_CMD_PHY_GET_CFG_RSP,
  /* Set data configuration request */
  SRV_PSERIAL_CMD_PHY_SET_CFG,
  /* Set data configuration response */
  SRV_PSERIAL_CMD_PHY_SET_CFG_RSP,
  /* Get command request */
  SRV_PSERIAL_CMD_PHY_CMD_CFG,
  /* Get command response */
  SRV_PSERIAL_CMD_PHY_CMD_CFG_RSP,
  /* Send message data */
  SRV_PSERIAL_CMD_PHY_SEND_MSG,
  /* Send message data response */
  SRV_PSERIAL_CMD_PHY_SEND_MSG_RSP,
  /* Receive message data */
  SRV_PSERIAL_CMD_PHY_RECEIVE_MSG,
  /* Noise capture request */
  SRV_PSERIAL_CMD_PHY_NOISE_REQ,
  /* Noise capture response */
  SRV_PSERIAL_CMD_PHY_NOISE_RSP,
  /* Get parameter list */
  SRV_PSERIAL_CMD_PHY_GET_CFG_LIST,
  /* Parameter list response */
  SRV_PSERIAL_CMD_PHY_GET_CFG_LIST_RSP,
  /* Reset PHY layer */
  SRV_PSERIAL_CMD_PHY_RESET_PHY_LAYER
} SRV_PSERIAL_COMMAND;
```

## Summary

List of PLC Serial Commands.

## Description

This enumeration defines the PLC commands used by PLC Phy Tester Tool provided by Microchip.
