DRupal Aggregation Bundles (DRAB)
====

Drupal provides a system to automatically aggregate CSS and JS files.   This is handy if you have a small site with a few CSS or JS files to be combined.    If you are working on a larger site with aggregation turned on, this means you are getting CSS/JS for sections of the site that a person may never see.

What this module allows us to do is create a "bundle" off CSS/JS files in the theme .info file and have that bundle applied only to certian paths.   

Another advantage of this approach is that it allows for fine grained control of the aggregated and cached files.  In the old system, one update to a file would mean a whole new big file to download.   In this approach, only the bundles that contain that file need to get updated.   This means the other bundles can remain in browser cache until the cache expires or a file in that bundle gets updated.

Example Use Case
----------------

Say you have a website for a large transportation provider like a airline or train service.   You would get to the homepage and most people would want to start booking a ticket.   The provider probably also has marketing/landing pages, user account management section, etc.

To optimize the amount of requests we can create a bundle for each section so instead of downloading the CSS/JS for the whole site which is more bandwidth they just get the correct aggregate files for the section they are in.   These files can be stored in the browser cache like normal so if they leave the section and then come back the files doesn't need to be downloaded again.

How to create a bundle
----------------------

Update your theme.info file with this array pattern

```php
bundle[base][css][files][] = base/css/logos.css.less
bundle[base][css][files][] = base/css/gateway.css.less
bundle[base][css][files][] = base/css/dropkick.css.less
bundle[base][css][files][] = base/css/shared-panes.css.less
bundle[base][css][files][] = base/css/jquery.ui.theme.css
bundle[base][css][files][] = base/css/jquery.ui.tabs.css
bundle[base][css][path][] = all

bundle[booking_path][css][files][] = transaction/css/step-1.css.less
bundle[booking_path][css][files][] = transaction/css/step-2.css.less
bundle[booking_path][css][files][] = transaction/css/step-3.css.less
bundle[booking_path][css][files][] = transaction/css/step-4.css.less
bundle[base][css][path][] = book/flights/*
```

Caveats
-------

This is for large sites that have descrete areas and not for small sites.

Because this is URL specific it makes it bit fragile.   If some site editor comes along and changes the URL structure so it doesn't match the patterns in the info file the site could break quite badly

