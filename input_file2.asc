2108003f	# addi $8, $8, 63		r8 = 63				$t0 = 0x0000003F
2129002a        # addi $9, $9, 42		r9 = 42				$t1 = 0x0000002A
212a0101        # addi $10, $9, 257		r10 = r9 + 257			$t2 = 0x0000012B
000a4824        # and $9, $0, $8                r9 = r0 & r8                    $t1 = 0
01485825        # or $11, $10, $8               r11 = r10 | r8                  $t3 = 0x0000013F
01486022        # sub $12, $10, $8              r12 = r10 - r8                  $t4 = 0x000000EC
3c09ffff	# lui $9, 65535			r9 = 65535 << 16		$t1 = 0xFFFF0000
292d000f	# slti $13, $9, 15		r13 = r9 < 15			$t5 = 1 
2d2d000f	# sltiu $13, $9, 15		r13 = r9 < 15 (unsigned)	$t5 = 0
8d0d0000	# lw $13, 0($8)			address not word aligned	halt = true
