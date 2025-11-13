/**
 * data_loader.h
 * Sample data initialization for GPS network
 */

#ifndef DATA_LOADER_H
#define DATA_LOADER_H

/**
 * Initialize basic Mumbai network (simple version)
 */
void load_basic_mumbai_network(void);

/**
 * Initialize enhanced Mumbai network with full metadata
 */
void load_enhanced_mumbai_network(void);

/**
 * Initialize custom network from user input
 */
void load_custom_network(void);

#endif // DATA_LOADER_H
