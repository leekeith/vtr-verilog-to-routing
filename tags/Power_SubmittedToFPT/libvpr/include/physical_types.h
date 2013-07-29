/*
 Data types describing the physical components on the FPGA architecture.

 Key data types:
 t_type_descriptor: describes a placeable complex logic block, 
 pb_type: describes the types of physical blocks within the t_type_descriptor in a hierarchy where the top block is the complex block and the leaf blocks implement one logical block
 pb_graph_node: is a flattened version of pb_type so a pb_type with 10 instances will have 10 pb_graph_nodes representing each instance
 pb: A specific physical block.  Shares a many-to-one relationship with a pb_graph_node.  For example, a circuit with 10 CLBs will have 10 CLB pbs and 1 CLB pb_graph_node, each CLB pb points to that single one pb_graph_node CLB.

 Date: February 19, 2009
 Authors: Jason Luu and Kenneth Kent
 */

#ifndef PHYSICAL_TYPES_H
#define PHYSICAL_TYPES_H

#include "logic_types.h"
#include "util.h"

/* Typedefs */
typedef struct s_power_usage t_power_usage;
typedef struct s_mux_node t_mux_node;
typedef struct s_power_arch t_power_arch;

/*************************************************************************************************/
/* FPGA basic definitions                                                                        */
/*************************************************************************************************/

/* Pins describe I/O into clustered logic block.  
 A pin may be unconnected, driving a net or in the fanout, respectively. */
enum e_pin_type {
	OPEN = -1, DRIVER = 0, RECEIVER = 1
};

/* Type of interconnect within complex block: Complete for everything connected (full crossbar), direct for one-to-one connections, and mux for many-to-one connections */
enum e_interconnect {
	COMPLETE_INTERC = 1, DIRECT_INTERC = 2, MUX_INTERC = 3
};

/* Orientations. */
enum e_side {
	TOP = 0, RIGHT = 1, BOTTOM = 2, LEFT = 3
};

/* pin location distributions */
enum e_pin_location_distr {
	E_SPREAD_PIN_DISTR = 1, E_CUSTOM_PIN_DISTR = 2
};

/* pb_type class */
enum e_pb_type_class {
	UNKNOWN_CLASS = 0, LUT_CLASS = 1, LATCH_CLASS = 2, MEMORY_CLASS = 3
};

/* Annotations for pin-to-pin connections */
enum e_pin_to_pin_annotation_type {
	E_ANNOT_PIN_TO_PIN_DELAY = 0,
	E_ANNOT_PIN_TO_PIN_CAPACITANCE,
	E_ANNOT_PIN_TO_PIN_PACK_PATTERN
};
enum e_pin_to_pin_annotation_format {
	E_ANNOT_PIN_TO_PIN_MATRIX = 0, E_ANNOT_PIN_TO_PIN_CONSTANT
};
enum e_pin_to_pin_delay_annotations {
	E_ANNOT_PIN_TO_PIN_DELAY_MIN = 0,
	E_ANNOT_PIN_TO_PIN_DELAY_MAX,
	E_ANNOT_PIN_TO_PIN_DELAY_TSETUP,
	E_ANNOT_PIN_TO_PIN_DELAY_CLOCK_TO_Q_MIN,
	E_ANNOT_PIN_TO_PIN_DELAY_CLOCK_TO_Q_MAX,
	E_ANNOT_PIN_TO_PIN_DELAY_THOLD
};
enum e_pin_to_pin_capacitance_annotations {
	E_ANNOT_PIN_TO_PIN_CAPACITANCE_C = 0
};
enum e_pin_to_pin_pack_pattern_annotations {
	E_ANNOT_PIN_TO_PIN_PACK_PATTERN_NAME = 0
};

enum e_power_leakage_type {
	LEAKAGE_UNDEFINED = 0, LEAKAGE_PROVIDED
};

enum e_power_dynamic_type {
	DYNAMIC_UNDEFINED = 0, DYNAMIC_C_INTERNAL, DYNAMIC_PROVIDED
};

enum e_encoding_type {
	ENCODING_ONE_HOT, ENCODING_DECODER
};

/*************************************************************************************************/
/* FPGA grid layout data types                                                                   */
/*************************************************************************************************/
/* Definition of how to place physical logic block in the grid 
 grid_loc_type - where the type goes and which numbers are valid
 start_col - the absolute value of the starting column from the left to fill, 
 used with COL_REPEAT
 repeat - the number of columns to skip before placing the same type, 
 used with COL_REPEAT.  0 means do not repeat
 rel_col - the fractional column to place type
 priority - in the event of conflict, which type gets picked?
 */
enum e_grid_loc_type {
	BOUNDARY = 0, FILL, COL_REPEAT, COL_REL
};
struct s_grid_loc_def {
	enum e_grid_loc_type grid_loc_type;
	int start_col;
	int repeat;
	float col_rel;
	int priority;
};

/* Data type definitions */
/*   Grid info */
struct s_clb_grid {
	boolean IsAuto;
	float Aspect;
	int W;
	int H;
};

/*************************************************************************************************/
/* FPGA Physical Logic Blocks data types                                                         */
/*************************************************************************************************/
struct s_power_usage {
	float dynamic;
	float leakage;
};

/* A class of CLB pins that share common properties
 * port_name: name of this class of pins
 * type:  DRIVER or RECEIVER (what is this pinclass?)              *
 * num_pins:  The number of logically equivalent pins forming this *
 *           class.                                                *
 * pinlist[]:  List of clb pin numbers which belong to this class. */
struct s_class {
	enum e_pin_type type;
	int num_pins;
	int *pinlist; /* [0..num_pins - 1] */
};
typedef struct s_class t_class;

/* Cluster timing delays:
 * C_ipin_cblock: Capacitance added to a routing track by the isolation     *
 *                buffer between a track and the Cblocks at an (i,j) loc.   *
 * T_ipin_cblock: Delay through an input pin connection box (from a         *
 *                   routing track to a logic block input pin).             */
typedef struct s_timing_inf {
	boolean timing_analysis_enabled;
	float C_ipin_cblock;
	float T_ipin_cblock;
} t_timing_inf;

struct s_pb_type;
/* declare before definition because pb_type contains modes and modes contain pb_types*/

/** Describes I/O and clock ports
 * name: name of the port
 * model_port: associated model port
 * is_clock: whether or not this port is a clock
 * num_pins: the number of pins this port has
 * parent_pb_type: pointer to the parent pb_type
 * port_class: port belongs to recognized set of ports in class library
 * index: port index by index in array of parent pb_type
 * port_index_by_type index of port by type (index by input, output, or clock)
 * equivalence: 
 */
struct s_port {
	char* name;
	t_model_ports *model_port;
	enum PORTS type;
	boolean is_clock;
	int num_pins;
	boolean equivalent;
	struct s_pb_type *parent_pb_type;
	char * port_class;

	int index;
	int port_index_by_type;
	char *chain_name;
};
typedef struct s_port t_port;

/** 
 * Info placed between pins that can be processed later for additional information
 * value: value/property pair
 * prop: value/property pair
 * type: type of annotation
 * format: formatting of data
 * input_pins: input pins as string affected by annotation
 * output_pins: output pins as string affected by annotation
 * clock_pin: clock as string affected by annotation
 */
struct s_pin_to_pin_annotation {
	char ** value; /* [0..num_value_prop_pairs - 1] */
	int * prop; /* [0..num_value_prop_pairs - 1] */
	int num_value_prop_pairs;

	enum e_pin_to_pin_annotation_type type;
	enum e_pin_to_pin_annotation_format format;

	char * input_pins;
	char * output_pins;
	char * clock;
};
typedef struct s_pin_to_pin_annotation t_pin_to_pin_annotation;

struct s_pb_graph_pin;

struct s_mux_node {
	int num_inputs;
	struct s_mux_node * children; /* [0..num_inputs-1] */
	int starting_pin_idx;
	int level;
	boolean level_restorer;
};

typedef struct s_mux_arch {
	int levels;
	int num_inputs;
	float * transistor_sizes; /* [0..levels] */
	enum e_encoding_type * encoding_types;
	t_mux_node * mux_graph_head;
} t_mux_arch;

struct s_pb_graph_edge;
/** Describes interconnect edge inside a cluster
 * type: type of the interconnect
 * name: indentifier for interconnect
 * input_string: input string verbatim to parse later
 * output_string: input string output to parse later
 * annotations: Annotations for delay, power, etc
 * num_annotations: Total number of annotations
 * infer_annotations: This interconnect is autogenerated, if true, 
 infer pack_patterns such as carry-chains and forced packs based on interconnect linked to it
 * parent_mode_index: Mode of parent as int
 */
struct s_interconnect {
	enum e_interconnect type;
	char *name;

	char *input_string;
	char *output_string;

	t_pin_to_pin_annotation *annotations; /* [0..num_annotations-1] */
	int num_annotations;
	boolean infer_annotations;

	int parent_mode_index;

	boolean port_info_initialized;
	int num_input_ports;
	int num_output_ports;
	int num_pins_per_port;
	t_mux_arch * mux_arch;

	float C_wire_in;
	float C_wire_out;

	float buffer_in_size;
	float buffer_out_size;

	t_power_usage power_usage;
};
typedef struct s_interconnect t_interconnect;

struct s_interconnect_pins {
	t_interconnect * interconnect;

	struct s_pb_graph_pin *** input_pins;
	struct s_pb_graph_pin *** output_pins;
};
typedef struct s_interconnect_pins t_interconnect_pins;

/** Describes mode
 * name: name of the mode
 * pb_type_children: pb_types it contains
 * interconnect: interconnect of parent pb_type to children pb_types or children to children pb_types
 * num_interconnect: Total number of interconnect tags specified by user
 * parent_pb_type: Which parent contains this mode
 * index: Index of mode in array with other modes
 */
struct s_mode {
	char* name;
	struct s_pb_type *pb_type_children; /* [0..num_child_pb_types] */
	int num_pb_type_children;
	t_interconnect *interconnect;
	int num_interconnect;
	struct s_pb_type *parent_pb_type;
	int index;

	t_power_usage power_usage;
};
typedef struct s_mode t_mode;

/* Identify pb pin type for timing purposes */
enum e_pb_graph_pin_type {
	PB_PIN_NORMAL = 0,
	PB_PIN_SEQUENTIAL,
	PB_PIN_INPAD,
	PB_PIN_OUTPAD,
	PB_PIN_TERMINAL,
	PB_PIN_CLOCK
};

/** Describes a pb graph pin
 * port: pointer to the port that this pin is associated with 
 * pin_number: pin number of the port that this pin is associated with
 * input edges: [0..num_input_edges - 1]edges incoming
 * num_input_edges: number edges incoming
 * output edges: [0..num_output_edges - 1]edges out_going
 * num_output_edges: number edges out_going
 * parent_node: parent pb_graph_node
 * pin_count_in_cluster: Unique number for pin inside cluster
 */
struct s_pb_graph_pin {
	t_port *port;
	int pin_number;
	struct s_pb_graph_edge** input_edges; /* [0..num_input_edges] */
	int num_input_edges;
	struct s_pb_graph_edge** output_edges; /* [0..num_output_edges] */
	int num_output_edges;

	struct s_pb_graph_node *parent_node;
	int pin_count_in_cluster;

	int scratch_pad; /* temporary data structure useful to store traversal info */

	/* timing information */
	enum e_pb_graph_pin_type type; /* Is a sequential logic element (TRUE), inpad/outpad (TRUE), or neither (FALSE) */
	float tsu_tco; /* For sequential logic elements, this is the setup time (if input) or clock-to-q time (if output) */
	struct s_pb_graph_pin** pin_timing; /* primitive ipin to opin timing */
	float *pin_timing_del_max; /* primitive ipin to opin timing */
	int num_pin_timing; /* primitive ipin to opin timing */

	/* Applies to clusters only */
	int pin_class;

	/* Applies to pins of primitive only */
	int *parent_pin_class; /* [0..depth-1] the grouping of pins that this particular pin belongs to */
	/* Applies to output pins of primitives only */
	struct s_pb_graph_pin ***list_of_connectable_input_pin_ptrs; /* [0..depth-1][0..num_connectable_primtive_input_pins-1] what input pins this output can connect to without exiting cluster at given depth */
	int *num_connectable_primtive_input_pins; /* [0..depth-1] number of input pins that this output pin can reach without exiting cluster at given depth */
};
typedef struct s_pb_graph_pin t_pb_graph_pin;

struct s_pb_graph_node;

/** Describes a pb graph edge, this is a "fat" edge which means it supports bused based connections
 * input_pins: array of pb_type graph input pins ptrs entering this edge
 * num_input_pins: Number of input pins entering this edge
 * output_pins: array of pb_type graph output pins ptrs entering this edge
 * num_output_pins: Number of output pins entering this edge
 */
struct s_pb_graph_edge {
	t_pb_graph_pin **input_pins;
	int num_input_pins;
	t_pb_graph_pin **output_pins;
	int num_output_pins;

	/* timing information */
	float delay_max;
	float delay_min;
	float capacitance;

	/* who drives this edge */
	t_interconnect * interconnect;
	int driver_set;
	int driver_pin;

	/* pack pattern info */
	char **pack_pattern_names; /*[0..num_pack_patterns(of_edge)-1]*/
	int *pack_pattern_indices; /*[0..num_pack_patterns(of_edge)-1]*/
	int num_pack_patterns;
	boolean infer_pattern; /*If TRUE, infer pattern based on patterns connected to it*/

};
typedef struct s_pb_graph_edge t_pb_graph_edge;

struct s_cluster_placement_primitive;

/* This structure stores the physical block graph nodes for a pb_type and mode of a cluster
 * pb_type: Pointer to the type of pb graph node this belongs to 
 * mode: parent mode of operation
 * placement_index: there are a certain number of pbs available, this gives the index of the node
 * child_pb_graph_nodes: array of children pb graph nodes organized into modes
 * parent_pb_graph_node: parent pb graph node
 */
struct s_pb_graph_node {
	const struct s_pb_type *pb_type;

	int placement_index;

	t_pb_graph_pin **input_pins; /* [0..num_input_ports-1] [0..num_port_pins-1]*/
	t_pb_graph_pin **output_pins; /* [0..num_output_ports-1] [0..num_port_pins-1]*/
	t_pb_graph_pin **clock_pins; /* [0..num_clock_ports-1] [0..num_port_pins-1]*/

	int num_input_ports;
	int num_output_ports;
	int num_clock_ports;

	int *num_input_pins; /* [0..num_input_ports - 1] */
	int *num_output_pins; /* [0..num_output_ports - 1] */
	int *num_clock_pins; /* [0..num_clock_ports - 1] */

	struct s_pb_graph_node ***child_pb_graph_nodes; /* [0..num_modes-1][0..num_pb_type_in_mode-1][0..num_pb-1] */
	struct s_pb_graph_node *parent_pb_graph_node;

	int total_pb_pins; /* only valid for top-level */\

	t_interconnect_pins ** interconnect_pins; /* [0..num_modes-1][0..num_interconnect_in_mode] */

	void *temp_scratch_pad; /* temporary data, useful for keeping track of things when traversing data structure */
	struct s_cluster_placement_primitive *cluster_placement_primitive; /* pointer to indexing structure useful during packing stage */

	int *input_pin_class_size; /* Stores the number of pins that belong to a particular input pin class */
	int num_input_pin_class; /* number of pin classes that this input pb_graph_node has */
	int *output_pin_class_size; /* Stores the number of pins that belong to a particular output pin class */
	int num_output_pin_class; /* number of output pin classes that this pb_graph_node has */
};
typedef struct s_pb_graph_node t_pb_graph_node;

/** Describes a physical block type
 * name: name of the physical block type
 * num_pb: maximum number of instances of this physical block type sharing one parent
 * blif_model: the string in the blif circuit that corresponds with this pb type
 * class_type: Special library name
 * modes: Different modes accepted
 * ports: I/O and clock ports
 * num_clock_pins: A count of the total number of clock pins
 * int num_input_pins: A count of the total number of input pins
 * int num_output_pins: A count of the total number of output pins
 * timing: Timing matrix of block [0..num_inputs-1][0..num_outputs-1]
 * parent_mode: mode of the parent block
 */
struct s_pb_type {
	char* name;
	int num_pb;
	char *blif_model;
	t_model *model;
	enum e_pb_type_class class_type;

	t_mode *modes; /* [0..num_modes-1] */
	int num_modes;
	t_port *ports; /* [0..num_ports] */
	int num_ports;

	int num_clock_pins;
	int num_input_pins; /* inputs not including clock pins */
	int num_output_pins;

	t_mode *parent_mode;
	int depth; /* depth of pb_type */

	float max_internal_delay;
	t_pin_to_pin_annotation *annotations; /* [0..num_annotations-1] */
	int num_annotations;

	/* Power - Jeff */
	enum e_power_dynamic_type power_dynamic_type;
	enum e_power_leakage_type power_leakage_type;

	float power_dynamic;
	float power_leakage;
	float C_internal;

	int leakage_default_mode;

	t_power_usage power_usage;
	float transistor_cnt;
	float transistor_cnt_interc;
};
typedef struct s_pb_type t_pb_type;

/* Describes the type for a physical logic block
 name: unique identifier for type  
 num_pins: Number of pins for the block
 capacity: Number of blocks of this type that can occupy one grid tile.
 This is primarily used for IO pads.
 height: Height of large block in grid tiles
 pinloc: Is set to 1 if a given pin exists on a certain position of a block.
 num_class: Number of logically-equivalent pin classes
 class_inf: Information of each logically-equivalent class
 pin_class: The class a pin belongs to
 is_global_pin: Whether or not a pin is global (hence not routed)
 is_Fc_frac: True if Fc fractional, else Fc absolute
 is_Fc_out_full_flex: True means opins will connect to all available segments
 pb_type: Internal subblocks and routing information for this physical block
 pb_graph_head: Head of DAG of pb_types_nodes and their edges

 area: Describes how much area this logic block takes, if undefined, use default
 type_timing_inf: timing information unique to this type
 num_drivers: Total number of output drivers supplied
 num_receivers: Total number of input receivers supplied
 index: Keep track of type in array for easy access
 */
struct s_type_descriptor /* TODO rename this.  maybe physical type descriptor or complex logic block or physical logic block*/
{
	char *name;
	int num_pins;
	int capacity;

	int height;

	int ***pinloc; /* [0..height-1][0..3][0..num_pins-1] */
	int *pin_height; /* [0..num_pins-1] */
	int **num_pin_loc_assignments; /* [0..height-1][0..3] */
	char ****pin_loc_assignments; /* [0..height-1][0..3][0..num_tokens-1][0..string_name] */
	enum e_pin_location_distr pin_location_distribution;

	int num_class;
	struct s_class *class_inf; /* [0..num_class-1] */
	int *pin_class; /* [0..num_pins-1] */

	boolean *is_global_pin; /* [0..num_pins-1] */

	boolean is_Fc_frac;
	boolean is_Fc_out_full_flex;
	float Fc_in;
	float Fc_out;

	/* Clustering info */
	struct s_pb_type *pb_type;
	t_pb_graph_node *pb_graph_head;

	/* Grid location info */
	struct s_grid_loc_def *grid_loc_def; /* [0..num_def-1] */
	int num_grid_loc_def;
	float area;

	/* This info can be determined from class_inf and pin_class but stored for faster access */
	int num_drivers;
	int num_receivers;

	int index; /* index of type descriptor in array (allows for index referencing) */
};
typedef struct s_type_descriptor t_type_descriptor;
typedef const struct s_type_descriptor *t_type_ptr;

/*************************************************************************************************/
/* FPGA Routing architecture                                                                     */
/*************************************************************************************************/

/* Description of routing channel distribution across the FPGA, only available for global routing
 * Width is standard dev. for Gaussian.  xpeak is where peak     *
 * occurs. dc is the dc offset for Gaussian and pulse waveforms. */
enum e_stat {
	UNIFORM, GAUSSIAN, PULSE, DELTA
};
typedef struct s_chan {
	enum e_stat type;
	float peak;
	float width;
	float xpeak;
	float dc;
} t_chan;

/* chan_width_io:  The relative width of the I/O channel between the pads    *
 *                 and logic array.                                          *
 * chan_x_dist: Describes the x-directed channel width distribution.         *
 * chan_y_dist: Describes the y-directed channel width distribution.         */
typedef struct s_chan_width_dist {
	float chan_width_io;
	t_chan chan_x_dist;
	t_chan chan_y_dist;
} t_chan_width_dist;

enum e_directionality {
	UNI_DIRECTIONAL, BI_DIRECTIONAL
};
enum e_switch_block_type {
	SUBSET, WILTON, UNIVERSAL, FULL
};
typedef enum e_switch_block_type t_switch_block_type;
enum e_Fc_type {
	ABSOLUTE, FRACTIONAL
};

/* Lists all the important information about a certain segment type.  Only   *
 * used if the route_type is DETAILED.  [0 .. det_routing_arch.num_segment]  *
 * frequency:  ratio of tracks which are of this segment type.            *
 * length:     Length (in clbs) of the segment.                              *
 * wire_switch:  Index of the switch type that connects other wires *to*     *
 *               this segment.                                               *
 * opin_switch:  Index of the switch type that connects output pins (OPINs)  *
 *               *to* this segment.                                          *
 * frac_cb:  The fraction of logic blocks along its length to which this     *
 *           segment can connect.  (i.e. internal population).               *
 * frac_sb:  The fraction of the length + 1 switch blocks along the segment  *
 *           to which the segment can connect.  Segments that aren't long    *
 *           lines must connect to at least two switch boxes.                *
 * Cmetal: Capacitance of a routing track, per unit logic block length.      *
 * Rmetal: Resistance of a routing track, per unit logic block length.       
 * (UDSD by AY) drivers: How do signals driving a routing track connect to   *
 *                       the track?                                          */
typedef struct s_segment_inf {
	int frequency;
	int length;
	short wire_switch;
	short opin_switch;
	float frac_cb;
	float frac_sb;
	boolean longline;
	float Rmetal;
	float Cmetal;
	enum e_directionality directionality;
	boolean *cb;
	int cb_len;
	boolean *sb;
	int sb_len;

	/* Power */
	float Cmetal_per_m;
} t_segment_inf;

/* Lists all the important information about a switch type.                  *
 * [0 .. det_routing_arch.num_switch]                                        *
 * buffered:  Does this switch include a buffer?                             *
 * R:  Equivalent resistance of the buffer/switch.                           *
 * Cin:  Input capacitance.                                                  *
 * Cout:  Output capacitance.                                                *
 * Tdel:  Intrinsic delay.  The delay through an unloaded switch is          *
 *        Tdel + R * Cout.                                                   *
 * mux_trans_size:  The area of each transistor in the segment's driving mux *
 *                  measured in minimum width transistor units               *
 * buf_size:  The area of the buffer. If set to zero, area should be         *
 *            calculated from R                                              */
typedef struct s_switch_inf t_switch_inf;
struct s_switch_inf {
	boolean buffered;
	float R;
	float Cin;
	float Cout;
	float Tdel;
	float mux_trans_size;
	float buf_size;
	char *name;

	boolean autosize_buffer;
	float buffer_last_stage_size;
};

struct poly_record {
	float Cpoly;
	float poly_extension;
};
/* Record for Poly Data
 Cpoly: poly capacitance
 poly_extention: poly extention
 */

/************************* POWER ***********************************/

/* two types of transisters */
typedef enum {
	NMOS, PMOS
} e_tx_type;

#define Q_charge 1.60e-19
#define vsat 8.00e4
#define Bolzmann_constant 1.38e-25

typedef struct s_transistor_inf t_transistor_inf;
typedef struct s_transistor_size_inf t_transistor_size_inf;

struct s_transistor_size_inf {
	float size;

	float leakage;
	float C_gate_cmos;
	float C_source_cmos;
	float C_drain_cmos;
	float C_gate_pass;
	float C_source_pass;
	float C_drain_pass;
};

struct s_transistor_inf {
	float Vth;
	int num_size_entries;
	t_transistor_size_inf * size_inf;
	t_transistor_size_inf * long_trans_inf;
};

/* Record for storing the technology parameters for NMOS and
 PMOS type of transistors

 min_length: minimum channel width of the transistor (in m)
 min_width:  minimum width of the transistor (in m)
 Vth:        threshold voltage (in volt)
 CJ:         junction capacitance (F/m^2)
 CJSW:       side-wall junction capacitance (F/m)
 CJSWG:      gate-edge side-wall bulk junction capacitance (F/m)
 CGDO:       gate-drain overlap capacitance (F/m)
 COX:        gate-oxide cpacitance per unit area
 EC:         contant for leakage current calculation
 */

struct temperature_record {
	int temperature;
	float NMOS_NFS_value;
	float PMOS_NFS_value;
};
/* record for storing the NMOS and PMOS NFS values for leakage current
 calculation
 temperature: temperature in degree celcius
 NMOS_NFS_value: NFS value for NMOS for that temperature
 PMOS_NFS_value: NFS value for PMOS for that temperature
 */

struct s_power_arch {
	float tech_size;
	float temperature;
	float Vdd;
	float C_wire_local;
	struct s_transistor_inf NMOS_tx_record;
	struct s_transistor_inf PMOS_tx_record;
	float short_circuit_power_percentage;
	float P_to_N_size_ratio;
};

typedef struct s_clock_inf t_clock_inf;
struct s_clock_inf {
	float density;
	float prob;
	boolean autosize_buffer;
	float buffer_size;
	float C_wire;
};

/*
 For Power calculation of the clock network
 Rbuffer: Resistance of clock buffer
 Cbuffer_in: Input Capacitance of clock buffer
 Cbuffer_out: Output Capacitance of clock buffer
 Rwire: Wire resistance of the clock network (unit: ohm per segment length)
 Cwire: Wire capacitance of the clock network (unit: F per segment length)
 Cin_per_clb_clock_pin: input capacitable to each logic block clock pin
 clock_density: transition density for the clock (usually is 2)
 */

typedef struct s_clocks t_clocks;
struct s_clocks {
	int num_global_clock;
	t_clock_inf *clock_inf; /* Details about the clock network */
};

/*   Detailed routing architecture */
typedef struct s_arch t_arch;
struct s_arch {
	t_chan_width_dist Chans;
	enum e_switch_block_type SBType;
	float R_minW_nmos;
	float R_minW_pmos;
	int Fs;
	float C_ipin_cblock;
	float T_ipin_cblock;
	float grid_logic_tile_area;
	float ipin_mux_trans_size;
	struct s_clb_grid clb_grid;
	t_segment_inf * Segments;
	int num_segments;
	struct s_switch_inf *Switches;
	int num_switches;
	t_model *models;
	t_model *model_library;
	t_power_arch *power;
	t_clocks * clocks;
};

#endif
