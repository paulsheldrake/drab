DRupal Aggregation Bundles (DRAB)
====

Drupal provides a system to automatically aggregate CSS and JS files.   This is handy if you have a small site with a few CSS or JS files to be combined.    IF you are working on a larger site with aggregation turned on, this means you are getting CSS/JS for sections of the site that a person may never see.

What this module allows us to do is create a "bundle" off CSS/JS files in the theme .info file and have that bundle applied only to certian paths.   

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
```

The path can be "all" or something like "book/flight/*"
