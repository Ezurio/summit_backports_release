#ifndef __BACKPORT_LINUX_THERMAL_H
#define __BACKPORT_LINUX_THERMAL_H
#include_next <linux/thermal.h>
#include <linux/version.h>

#ifdef CONFIG_THERMAL
#if LINUX_VERSION_IS_LESS(3,8,0)
#include <linux/errno.h>

struct thermal_bind_params {
	struct thermal_cooling_device *cdev;
	int weight;
	int trip_mask;
	int (*match)(struct thermal_zone_device *tz,
		     struct thermal_cooling_device *cdev);
};

struct thermal_zone_params {
	int num_tbps;
	struct thermal_bind_params *tbp;
};

static inline struct thermal_zone_device *
backport_thermal_zone_device_register(const char *type, int trips, int mask,
				      void *devdata,
				      struct thermal_zone_device_ops *ops,
				      const struct thermal_zone_params *tzp,
				      int passive_delay, int polling_delay)
{
	return ERR_PTR(-EOPNOTSUPP);
}
#define thermal_zone_device_register backport_thermal_zone_device_register

static inline void thermal_notify_framework(struct thermal_zone_device *tz,
	int trip)
{ }
#else /* < 3.8.0 */

#if LINUX_VERSION_IS_LESS(3,10,0)
#define thermal_notify_framework notify_thermal_framework
#endif /* LINUX_VERSION_IS_LESS(3,10,0) */

#if LINUX_VERSION_IS_LESS(4,3,0)

typedef struct thermal_zone_device_ops old_thermal_zone_device_ops_t;

/* also add a way to call the old register and unregister functions */
static inline struct thermal_zone_device *old_thermal_zone_device_register(
	const char *type, int trips, int mask, void *devdata,
	old_thermal_zone_device_ops_t *_ops,
	const struct thermal_zone_params *_tzp,
	int passive_delay, int polling_delay)
{
	struct thermal_zone_device_ops *ops =
		(struct thermal_zone_device_ops *) _ops;

	/* cast the const away */
	struct thermal_zone_params *tzp =
		(struct thermal_zone_params *)_tzp;

	return thermal_zone_device_register(type, trips, mask, devdata,
					    ops, tzp, passive_delay,
					    polling_delay);
}

static inline
void old_thermal_zone_device_unregister(struct thermal_zone_device *dev)
{
	thermal_zone_device_unregister(dev);
}

struct backport_thermal_zone_device_ops {
	int (*bind) (struct thermal_zone_device *,
		     struct thermal_cooling_device *);
	int (*unbind) (struct thermal_zone_device *,
		       struct thermal_cooling_device *);
	int (*get_temp) (struct thermal_zone_device *, int *);
	int (*get_mode) (struct thermal_zone_device *,
			 enum thermal_device_mode *);
	int (*set_mode) (struct thermal_zone_device *,
		enum thermal_device_mode);
	int (*get_trip_type) (struct thermal_zone_device *, int,
		enum thermal_trip_type *);
	int (*get_trip_temp) (struct thermal_zone_device *, int, int *);
	int (*set_trip_temp) (struct thermal_zone_device *, int, int);
	int (*get_trip_hyst) (struct thermal_zone_device *, int, int *);
	int (*set_trip_hyst) (struct thermal_zone_device *, int, int);
	int (*get_crit_temp) (struct thermal_zone_device *, int *);
	int (*set_emul_temp) (struct thermal_zone_device *, int);
	int (*get_trend) (struct thermal_zone_device *, int,
			  enum thermal_trend *);
	int (*notify) (struct thermal_zone_device *, int,
		       enum thermal_trip_type);
};
#define thermal_zone_device_ops LINUX_BACKPORT(thermal_zone_device_ops)

#undef thermal_zone_device_register
struct thermal_zone_device *backport_thermal_zone_device_register(
	const char *type, int trips, int mask, void *devdata,
	struct thermal_zone_device_ops *ops,
	const struct thermal_zone_params *tzp,
	int passive_delay, int polling_delay);

#define thermal_zone_device_register \
	LINUX_BACKPORT(thermal_zone_device_register)

#undef thermal_zone_device_unregister
void backport_thermal_zone_device_unregister(struct thermal_zone_device *);
#define thermal_zone_device_unregister			\
	LINUX_BACKPORT(thermal_zone_device_unregister)

#endif /* LINUX_VERSION_IS_LESS(4,3,0) */
#endif /* ! < 3.8.0 */

#if LINUX_VERSION_IS_LESS(5,9,0)
#define thermal_zone_device_enable LINUX_BACKPORT(thermal_zone_device_enable)
static inline int thermal_zone_device_enable(struct thermal_zone_device *tz)
{ return 0; }

#define thermal_zone_device_disable LINUX_BACKPORT(thermal_zone_device_disable)
static inline int thermal_zone_device_disable(struct thermal_zone_device *tz)
{ return 0; }
#endif /* < 5.9 */

#if LINUX_VERSION_IS_LESS(4,9,0)
/* Thermal notification reason */
enum thermal_notify_event {
	THERMAL_EVENT_UNSPECIFIED, /* Unspecified event */
	THERMAL_EVENT_TEMP_SAMPLE, /* New Temperature sample */
	THERMAL_TRIP_VIOLATED, /* TRIP Point violation */
	THERMAL_TRIP_CHANGED, /* TRIP Point temperature changed */
	THERMAL_DEVICE_DOWN, /* Thermal device is down */
	THERMAL_DEVICE_UP, /* Thermal device is up after a down event */
	THERMAL_DEVICE_POWER_CAPABILITY_CHANGED, /* power capability changed */
	THERMAL_TABLE_CHANGED, /* Thermal table(s) changed */
	THERMAL_EVENT_KEEP_ALIVE, /* Request for user space handler to respond */
};

static inline void
backport_thermal_zone_device_update(struct thermal_zone_device *tz,
				    enum thermal_notify_event event)
{
	thermal_zone_device_update(tz);
}
#define thermal_zone_device_update LINUX_BACKPORT(thermal_zone_device_update)
#endif /* < 4.9 */

#if LINUX_VERSION_IS_LESS(3,19,0) || \
	(LINUX_VERSION_IN_RANGE(5,8,0, 6,0,0) && \
	!(LINUX_VERSION_IS_EQU(5,14,0) && RHEL_RELEASE_IS_GEQ(9,5)))
struct thermal_trip {
	struct device_node *np;
	int temperature;
	int hysteresis;
	enum thermal_trip_type type;
};
#endif /* < 3.19 or 5.8.0-5.9.0 */

/* for < 6,0,0 the trips are invalid anyway */
#if LINUX_VERSION_IS_LESS(6,0,0) && \
	!(LINUX_VERSION_IS_EQU(5,14,0) && RHEL_RELEASE_IS_GEQ(9,5))
#define for_each_thermal_trip LINUX_BACKPORT(for_each_thermal_trip)
static inline
int for_each_thermal_trip(struct thermal_zone_device *tz,
			  int (*cb)(struct thermal_trip *, void *),
			  void *data)
{
	return 0;
}
#elif LINUX_VERSION_IS_LESS(6,6,0)
#define for_each_thermal_trip LINUX_BACKPORT(for_each_thermal_trip)
static inline
int for_each_thermal_trip(struct thermal_zone_device *tz,
			  int (*cb)(struct thermal_trip *, void *),
			  void *data)
{
	struct thermal_trip *trip;
	struct thermal_trip *trips = (void *)tz->trips;
	int ret;

	for (trip = trips; trip - trips < tz->num_trips; trip++) {
		ret = cb(trip, data);
		if (ret)
			return ret;
	}

	return 0;
}
#endif

#if LINUX_VERSION_IS_LESS(6,4,0)
#define thermal_zone_device_priv LINUX_BACKPORT(thermal_zone_device_priv)
static inline void *thermal_zone_device_priv(struct thermal_zone_device *tzd)
{
	return tzd->devdata;
}
#endif /* < 6.4.0 */

#if LINUX_VERSION_IS_LESS(6,9,0)
static inline struct thermal_zone_device *
backport_thermal_zone_device_register_with_trips(const char *type,
					struct thermal_trip *trips,
					int num_trips, void *devdata,
					struct thermal_zone_device_ops *ops,
					struct thermal_zone_params *tzp,
					int passive_delay,
					int polling_delay)
{
#if LINUX_VERSION_IS_LESS(6,0,0)
	return thermal_zone_device_register(type, num_trips, 0, devdata, ops, tzp,
						passive_delay, polling_delay);
#else
	return thermal_zone_device_register_with_trips(type,
								(struct thermal_trip *)(void *) trips,
								num_trips,
								0, devdata,
								ops, tzp, passive_delay,
								polling_delay);
#endif /* < 6.0 */
}
#define thermal_zone_device_register_with_trips LINUX_BACKPORT(thermal_zone_device_register_with_trips)
#endif /* < 6.9 */

#endif /* CONFIG_THERMAL */

#endif /* __BACKPORT_LINUX_THERMAL_H */
