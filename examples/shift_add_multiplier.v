module shift_add_multiplier(
  input clk,
  input reset,
  input in_data_valid,
  input[7:0] in_data1,
  input[7:0] in_data2,
  output reg busy,

  output reg out_data_valid,
  output reg[15:0] out_data
);

reg[7:0] data1;
reg[15:0] data2;
reg[15:0] result;

always@(posedge clk) begin
  if(reset) begin
    data1 <= 0;
    data2 <= 0;
    result <= 0;
    busy <= 0;
    out_data_valid <= 0;
  end
  else if(in_data_valid) begin
    data1 <= in_data1;
    data2 <= {8'b0, in_data2};
    result <= 0;
    busy <= 1;
  end 
  else if(busy) begin
    if(data1[0]) begin
      result <= result + data2;
    end
    if(data1 != 0) begin
      data1 <= data1 >> 1;
      data2 <= data2 << 1;
    end else begin
      out_data_valid <= 1;
      out_data <= result;
      busy <= 0;
    end
  end else begin
    out_data_valid <= 0;
    out_data <= 0;
    result <= 0;
  end
end

endmodule
