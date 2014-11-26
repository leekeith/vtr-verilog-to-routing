/*------------------------------------------------------------------------------
 * This code was generated by Spiral Multiplier Block Generator, www.spiral.net
 * Copyright (c) 2006, Carnegie Mellon University
 * All rights reserved.
 * The code is distributed under a BSD style license
 * (see http://www.opensource.org/licenses/bsd-license.php)
 *------------------------------------------------------------------------------ */
/* ./multBlockGen.pl 27564 -fractionalBits 0*/
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
    w256,
    w255,
    w2040,
    w2295,
    w2,
    w2297,
    w9188,
    w6891,
    w27564;

  assign w1 = i_data0;
  assign w2 = w1 << 1;
  assign w2040 = w255 << 3;
  assign w2295 = w255 + w2040;
  assign w2297 = w2295 + w2;
  assign w255 = w256 - w1;
  assign w256 = w1 << 8;
  assign w27564 = w6891 << 2;
  assign w6891 = w9188 - w2297;
  assign w9188 = w2297 << 2;

  assign o_data0 = w27564;

  //multiplier_block area estimate = 6915.58560096765;
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
