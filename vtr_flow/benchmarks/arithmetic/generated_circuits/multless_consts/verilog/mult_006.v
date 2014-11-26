/*------------------------------------------------------------------------------
 * This code was generated by Spiral Multiplier Block Generator, www.spiral.net
 * Copyright (c) 2006, Carnegie Mellon University
 * All rights reserved.
 * The code is distributed under a BSD style license
 * (see http://www.opensource.org/licenses/bsd-license.php)
 *------------------------------------------------------------------------------ */
/* ./multBlockGen.pl 7117 -fractionalBits 0*/
module multiplier_block (
    i_data0,
    o_data0
);

  // Port mode declarations:
  input   [31:0] i_data0;
  output  [31:0]
    o_data0;

  //Multipliers:

  wire [31:0]
    w1,
    w512,
    w513,
    w4104,
    w3591,
    w64,
    w65,
    w7182,
    w7117;

  assign w1 = i_data0;
  assign w3591 = w4104 - w513;
  assign w4104 = w513 << 3;
  assign w512 = w1 << 9;
  assign w513 = w1 + w512;
  assign w64 = w1 << 6;
  assign w65 = w1 + w64;
  assign w7117 = w7182 - w65;
  assign w7182 = w3591 << 1;

  assign o_data0 = w7117;

  //multiplier_block area estimate = 7497.70553614559;
endmodule //multiplier_block

module surround_with_regs(
	i_data0,
	o_data0,
	clk
);

	// Port mode declarations:
	input   [31:0] i_data0;
	output  [31:0] o_data0;
	reg  [31:0] o_data0;
	input clk;

	reg [31:0] i_data0_reg;
	wire [30:0] o_data0_from_mult;

	always @(posedge clk) begin
		i_data0_reg <= i_data0;
		o_data0 <= o_data0_from_mult;
	end

	multiplier_block mult_blk(
		.i_data0(i_data0_reg),
		.o_data0(o_data0_from_mult)
	);

endmodule
