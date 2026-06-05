/*
 * SAFE LIVE DEMO CANDIDATE.
 * Header for passive visibility/pair-audit style reporting.
 */

#ifndef BLUETOOTH_AUDIT_H
#define BLUETOOTH_AUDIT_H

void bluetoothPairAuditAction();
void bluetoothPairAuditResult();
void bluetoothPairAuditAfterInput(const char* inputText);

#endif
