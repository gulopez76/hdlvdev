<?php
/**
 * Implementation of hook_block().
 */
function uc_decimal_quantities_block($op = 'list', $delta = 0, $edit = array()) {
  global $user;

  switch ($op) {
    case 'list':
      $blocks = array();

      // TODO: Add sensible default settings for the cart block based on the
      // docs at http://api.drupal.org/api/function/hook_block/6.
      $blocks[0] = array(
        'info' => t('Shopping cart (with decimal quantities enabled)'),
        'cache' => BLOCK_NO_CACHE,
      );

      return $blocks;

    case 'configure':
      // 0 = Default shopping cart block.
      if ($delta == 0) {
        module_load_include('inc', 'uc_cart', 'uc_cart');
        return uc_cart_block_settings_form();
      }
      break;

    case 'save':
      // 0 = Default shopping cart block.
      if ($delta == 0) {
        module_load_include('inc', 'uc_cart', 'uc_cart');
        uc_cart_block_settings_form_submit($edit);
      }
      break;

    case 'view':
      // 0 = Default shopping cart block.
      if ($delta == 0) {
        module_load_include('inc', 'uc_cart', 'uc_cart');
        $cachable = !$user->uid && variable_get('cache', CACHE_DISABLED) != CACHE_DISABLED;
        $uc_cart_path = drupal_get_path('module', 'uc_cart');
        $product_count = count(uc_cart_get_contents());

        // Display nothing if the block is set to hide on empty and there are no
        // items in the cart.
        if (!$cachable && variable_get('uc_cart_block_empty_hide', FALSE) && !$product_count) {
          return;
        }

        // Add the cart block CSS.
        drupal_add_css(drupal_get_path('module', 'uc_cart') .'/uc_cart_block.css');

        // If the block is collapsible, add the appropriate JS.
        if (!$cachable && variable_get('uc_cart_block_collapsible', TRUE)) {
          drupal_add_js(array('ucCollapsedBlock' => variable_get('uc_cart_block_collapsed', TRUE)), 'setting');
          drupal_add_js(drupal_get_path('module', 'uc_cart') .'/uc_cart_block.js');
          $collapsible = TRUE;
        }
        else {
          $collapsible = FALSE;
        }

        // Hack in some CSS to hide the block if necessary. drupal_add_css() is in Drupal 7 for this...
        if (variable_get('uc_cart_block_collapsed', TRUE)) {
          drupal_set_html_head("<style type='text/css'>#cart-block-contents { display: none; }</style>");
        }

        // Build the cart image if enabled.
        if (variable_get('uc_cart_block_image', TRUE)) {
          // If the cart is empty or we need a cachable cart block...
          if ($cachable || !$product_count) {
            // Use the "empty" cart icon.
            $icon_class = 'cart-block-icon-empty';
          }
          else {
            // Otherwise use the "full" cart icon.
            $icon_class = 'cart-block-icon-full';
          }
        }
        else {
          $icon_class = FALSE;
        }

        // Set the text of the block title.
        $title = check_plain(trim(variable_get('uc_cart_block_title', '')));

        if (empty($title)) {
          $title = t('Shopping cart');
        }

        // Build the block array for display based on cache settings.
        if ($cachable) {
          // Caching is turned on and the user is not logged in, so we should
          // deliver a block that is safe for caching.
          $block = array(
            'subject' => theme('uc_cart_block_title', $title, $icon_class, $collapsible),
            'content' => theme('uc_cart_block_content_cachable'),
          );
        }
        else {
          // Otherwise build the whole shebang.

          // First build the help text.
          $help_text = FALSE;

          if (variable_get('uc_cart_show_help_text', FALSE) && ($text = variable_get('uc_cart_help_text', t('Click title to display cart contents.')))) {
            $help_text = check_plain($text);
          }

          $items = FALSE;
          $item_count = 0;
          $total = 0;

          if ($product_count) {
            foreach (uc_cart_get_contents() as $item) {
              if (_product_allows_decimal_quantities($item)) {
                $item->qty /= UC_DECIMAL_QUANTITIES_FACTOR;
                $item->price *= UC_DECIMAL_QUANTITIES_FACTOR;
                $display_item = module_invoke($item->module, 'cart_display', $item);
                if (!empty($display_item)) {
                  $items[] = array(
                    'nid' => $display_item['nid']['#value'],
                    'qty' => t('@qty@unit', array(
                      '@qty' => $display_item['qty']['#default_value'],
                      '@unit' => UC_DECIMAL_QUANTITIES_UNIT,
                    )),
                    'title' => $display_item['title']['#value'],
                    'price' => $display_item['#total'],
                    'desc' => isset($display_item['description']['#value']) ? $display_item['description']['#value'] : FALSE,
                  );
                }
                ++$item_count;
              }
              else {
                $display_item = module_invoke($item->module, 'cart_display', $item);
                if (!empty($display_item)) {
                  $items[] = array(
                    'nid' => $display_item['nid']['#value'],
                    'qty' => t('@qty&times;', array('@qty' => $display_item['qty']['#default_value'])),
                    'title' => $display_item['title']['#value'],
                    'price' => $display_item['#total'],
                    'desc' => isset($display_item['description']['#value']) ? $display_item['description']['#value'] : FALSE,
                  );
                }
                $item_count += $item->qty;
              }

              $total += $display_item['#total'];
            }
          }

          // Build the item count text and cart links.
          $item_text = format_plural($item_count, '<span class="num-items">@count</span> Item', '<span class="num-items">@count</span> Items');

          $summary_links = array(
            'cart-block-view-cart' => array(
              'title' => t('View cart'),
              'href' => 'cart',
              'attributes' => array('rel' => 'nofollow'),
            ),
          );

          // Only add the checkout link if checkout is enabled.
          if (variable_get('uc_checkout_enabled', TRUE)) {
            $summary_links['cart-block-checkout'] = array(
              'title' => t('Checkout'),
              'href' => 'cart/checkout',
              'attributes' => array('rel' => 'nofollow'),
            );
          }

          $block = array(
            'subject' => theme('uc_cart_block_title', $title, $icon_class, $collapsible),
            'content' => theme('uc_cart_block_content', $help_text, $items, $item_count, $item_text, $total, $summary_links),
          );
        }

        return $block;
      }
      break;
  }
}