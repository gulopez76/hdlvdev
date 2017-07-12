<?php
function uc_decimal_quantities_gst_cart_review_table($show_subtotal = TRUE) {

  $subtotal = 0;
  $subtotal_tax = 0;
  $subtotal_nontax = 0;

  // Set up table header.
  $header = array(
    array('data' => t('Qty'), 'class' => 'qty'),
    array('data' => t('Products'), 'class' => 'products'),
    array('data' => t('Price'), 'class' => 'price'),
  );
  $header[] = array('data' => t('GST'), 'class' => 'price');
  $header[] = array('data' => t('Total'), 'class' => 'price');

  $context = array(
    'location' => 'cart-checkout-item',
  );

  // Set up table rows.
  $contents = uc_cart_get_contents();
  foreach ($contents as $item) {
    $node = node_load($item->nid);
    $price_info = array(
      'price' => $item->price,
      'qty' => $item->qty,
    );
    $context = array(
      'type' => 'cart_item',
      'revision' => 'altered',
      'subject' => array(
        'cart' => $contents,
        'cart_item' => $item,
        'node' => $node,
      ),
    );

    $total = uc_price($price_info, $context);
    $tax_total = 0;
    foreach (uc_taxes_rate_load() as $tax) {
      if (in_array($node->type, $tax->taxed_product_types) && ($tax->shippable == 0 || $node->shippable == 1)) {
        $tax_total += $total - $total / (1 + $tax->rate);
      }
    }
    $price = $total - $tax_total;

    $subtotal_nontax += $price;
    $subtotal_tax += $tax_total;
    $subtotal += $total;

    $description = check_plain($item->title);
    foreach (module_implements('cart_item_description') as $module) {
      $description .= module_invoke($module, 'cart_item_description', $item);
    }

    // Remove node from context to prevent the price from being altered.
    unset($context['subject']);
    $context['type'] = 'amount';
    $context['revision'] = 'themed-original';
    if (_product_allows_decimal_quantities($item)) {
      $quantity_cell = array(
        'data' => t('@qty@unit', array(
          '@qty' => number_format($item->qty / UC_DECIMAL_QUANTITIES_FACTOR, 2),
          '@unit' => UC_DECIMAL_QUANTITIES_UNIT,
        )),
        'class' => 'qty',
      );
    }
    else {
      $quantity_cell = array(
        'data' => t('@qty&times;', array(
          '@qty' => $item->qty)
        ),
        'class' => 'qty'
      );
    }

    $base_array = array(
     $quantity_cell,
      array('data' => $description, 'class' => 'products'),
      array('data' => uc_price($price, $context), 'class' => 'price'),
    );
    $gst_array = array(
      array('data' => uc_price($tax_total, $context), 'class' => 'price'),
      array('data' => uc_price($total, $context), 'class' => 'price'),
    );
    $rows[] = array_merge($base_array, $gst_array);
  }

  // Add the subtotal as the final row.
  if ($show_subtotal) {
    $context = array(
      'type' => 'amount',
      'revision' => 'themed-original',
    );
    $rows[] = array(
      'data' => array(
        array('data' => '<span id="subtotal-title">' . t('Subtotals:') . '</span> ' . uc_price($subtotal_nontax, $context), 'colspan' => 3, 'class' => 'subtotal'),
        array('data' => uc_price($subtotal_tax, $context)),
        array('data' => uc_price($subtotal, $context)),
      ),
      'class' => 'subtotal',
    );
  }

  return theme('table', $header, $rows, array('class' => 'cart-review'));
}