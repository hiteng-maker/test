#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/stat.h> // read/write permissions
#include <linux/init.h>
#include <linux/kernel.h>

volatile int temp= 0, pressure=0;     // to be read/written from/in main memory

/*********************************** Show funtion prototypes for Both attributes*************************************/


static ssize_t temp_show( struct device *dev, struct device_attribute *attr, char *buffer);  // read
static ssize_t pressure_show( struct device *dev, struct device_attribute *attr, char *buffer);

/******************* Both functions will be called by DEVICE_ATTR macro for respective attributes******************/

static ssize_t temp_show( struct device *dev, struct device_attribute *attr, char *buffer)
{
	printk(KERN_INFO "The current value of temp is %d\n",temp);
	return sprintf(buffer, "%d", temp);
}
static ssize_t pressure_show( struct device *dev, struct device_attribute *attr, char *buffer)	
{
	printk(KERN_INFO "The current value of pressure is %d\n",pressure);
	return sprintf(buffer, "%d", pressure);
}


/*************** attributes are declared using below MACRO where we have declared two class attributes pressure and temp*************/

static DEVICE_ATTR(pressure, 0660, pressure_show, NULL);  // (name, mode , show , store)
static DEVICE_ATTR(temp, 0660, temp_show, NULL);

/* The Above two lines will declare two structures of type struct device_attribute with respective names 
   dev_attr_pressure and dev_attr_temp. 

  Now, These two attributes can be organized as follows into a group. */


static struct attribute *mysensor_attributes[] = {
	&dev_attr_pressure.attr,
	&dev_attr_temp.attr,
	NULL
};
/***************** An attribute_group stores the null terminated array of struct attribute pointers***************************/

static const struct attribute_group my_attr_group = {
	.attrs = mysensor_attributes,
};

/********************** i2c_probe() will be called after it matches compatible property in device tree **********************/

static int i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int file;
	printk(KERN_ERR "++++++++++++++++++++ sensor probe function +++++++++++++++++\n");

	file = sysfs_create_group(&client->dev.kobj, &my_attr_group);

	if(file)
		sysfs_remove_group(&client->dev.kobj, &my_attr_group);
	
	return file;
}

static int i2c_remove(struct i2c_client *client)
{
	sysfs_remove_group(&client->dev.kobj, &my_attr_group);
	return 0;
}

/********** i2c_device_id structure is referenced by the i2c_driver structure which specifically used for i2c devices*******/


static const struct i2c_device_id my_i2c_id[] = {
        {"myi2cdev", 0},			  /** To find specific device mentioned in our device tree.
							probe function will be called only after matching this property in device tree ***/
        {}						
};

/*** MODULE_DEVICE_TABLE() macro used to determine which kernel configuration options are needed for specific hardware.
	This Macro is must to use because it is responsible for module loading and initializing *****/

MODULE_DEVICE_TABLE(i2c, my_i2c_id);            

/*************** Hardware description data is used to configure the device *****************/

static const struct of_device_id my_of_match[] = {
        { .compatible = "myi2cdev" },             
        {},
};

MODULE_DEVICE_TABLE(of, my_of_match);

/****************** i2c driver structure represents an i2c device driver**********************/

static struct i2c_driver my_i2c_driver = {
	.driver = {
		.name = "me782",
		.owner = THIS_MODULE,
		.of_match_table = my_of_match,
},
	.probe = i2c_probe,					// callback for device binding
	.remove = i2c_remove,					// callback for device unbinding
	.id_table = my_i2c_id,					// List of i2c devices supported by this driver
};

/*************** Macro used to registering a Module I2C driver***********/

module_i2c_driver(my_i2c_driver);   // This will eliminates calling of module_init and module_exit


MODULE_AUTHOR("HITEN_GONDALIYA");
MODULE_DESCRIPTION("BASIC I2C bus adapter");
MODULE_LICENSE("GPL v2");
