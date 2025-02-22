// Copyright (c) .NET Foundation and Contributors.  All Rights Reserved.  See LICENSE in the project root for license information.
#include "THSTensor.h"

#include <iostream>
#include <fstream>



Tensor THSTensor_bernoulli(const Tensor tensor, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? tensor->bernoulli() : tensor->bernoulli(*gen));
}

Tensor THSTensor_bernoulli_0(Tensor tensor, const double p, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? tensor->bernoulli_(p) : tensor->bernoulli_(p, *gen));
}

Tensor THSTensor_bernoulli_1(Tensor tensor, const Tensor p_tensor, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? tensor->bernoulli_(*p_tensor) : tensor->bernoulli_(*p_tensor, *gen));
}

Tensor THSTensor_binomial(Tensor tensor, const Tensor p_tensor, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? torch::binomial(*tensor, *p_tensor) : torch::binomial(*tensor, *p_tensor, *gen));
}

Tensor THSTensor_cauchy_(Tensor tensor, const double median, const double sigma, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? tensor->cauchy_(median, sigma) : tensor->cauchy_(median, sigma, *gen));
}

Tensor THSTensor_exponential_(Tensor tensor, const double lambda, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? tensor->exponential_(lambda) : tensor->exponential_(lambda, *gen));
}

Tensor THSTensor_geometric_(Tensor tensor, const double p, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? tensor->geometric_(p) : tensor->geometric_(p, *gen));
}

Tensor THSTensor_log_normal_(Tensor tensor, double mean, double std, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? tensor->log_normal_(mean, std) : tensor->log_normal_(mean, std, *gen));
}

Tensor THSTensor_multinomial(const Tensor tensor, const int64_t num_samples, const bool replacement, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? tensor->multinomial(num_samples, replacement) : tensor->multinomial(num_samples, replacement, *gen));
}

Tensor THSTensor_poisson(const Tensor tensor, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? torch::poisson(*tensor) : torch::poisson(*tensor, *gen))
}

Tensor THSInit_normal_(Tensor tensor, double mean, double std)
{
    CATCH_TENSOR(torch::nn::init::normal_(*tensor, mean, std))
}

Tensor THSTensor_normal_(Tensor tensor, double mean, double std, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? tensor->normal_(mean, std) : tensor->normal_(mean, std, *gen));
}

double norm_cdf(double x) { return (1.0 + erf(x / sqrt(2.0))) / 2.0; }

Tensor THSInit_trunc_normal_(Tensor tensor, double mean, double std, double a, double b)
{
    CATCH(
        auto l = norm_cdf((a - mean) / std);
        auto u = norm_cdf((b - mean) / std);

        auto m = torch::Scalar(mean);
        auto s = torch::Scalar(std);

        auto a_ = torch::Scalar(a);
        auto b_ = torch::Scalar(b);

        THSInit_uniform_(tensor, 2 * l - 1, 2 * u - 1);

        tensor->erfinv_();
        tensor->mul_(m);
        tensor->add_(s);
        tensor->clamp_(a_, b_);
    );

    return ResultTensor(*tensor);
}

Tensor THSTensor_rand(
    const Generator gen,
    const int64_t* sizes,
    const int length,
    const int8_t scalar_type,
    const int device_type, const int device_index,
    const bool requires_grad)
{
    Tensor tensor;
    CATCH(
        auto options = at::TensorOptions()
        .dtype(at::ScalarType(scalar_type))
        .device(c10::Device((c10::DeviceType)device_type, (c10::DeviceIndex)device_index))
        .requires_grad(requires_grad);

        tensor = new torch::Tensor(gen == nullptr ? torch::rand(at::ArrayRef<int64_t>(sizes, length), options) : torch::rand(at::ArrayRef<int64_t>(sizes, length), *gen, options));
    );
    return tensor;
}

Tensor THSTensor_rand_out(const Generator gen, const int64_t* sizes, const int length, const Tensor out)
{
    CATCH_TENSOR(gen == nullptr ? torch::rand_out(*out, at::ArrayRef<int64_t>(sizes, length)) : torch::rand_out(*out, at::ArrayRef<int64_t>(sizes, length), *gen));
}

Tensor THSTensor_rand_like(
    const Tensor input,
    const int8_t scalar_type,
    const int device_type, const int device_index,
    const bool requires_grad)
{
    auto options = at::TensorOptions()
        .dtype(at::ScalarType(scalar_type))
        .device(c10::Device((c10::DeviceType)device_type, (c10::DeviceIndex)device_index))
        .requires_grad(requires_grad);

    CATCH_TENSOR(torch::rand_like(*input, options));
}

Tensor THSTensor_randint(
    const Generator gen,
    const int64_t low,
    const int64_t high,
    const int64_t* sizes,
    const int length,
    const int8_t scalar_type,
    const int device_type, const int device_index,
    const bool requires_grad)
{
    try {
        torch_last_err = 0;
        auto options = at::TensorOptions()
            .dtype(at::ScalarType(scalar_type))
            .device(c10::Device((c10::DeviceType)device_type, (c10::DeviceIndex)device_index))
            .requires_grad(requires_grad);

        auto result = (gen == nullptr)
            ? torch::randint(low, high, at::ArrayRef<int64_t>(sizes, length), options)
            : torch::randint(low, high, at::ArrayRef<int64_t>(sizes, length), *gen, options);

        return ResultTensor(result);
    }
    catch (const c10::Error e) {
        torch_last_err = strdup(e.what());
    }
    catch (const std::runtime_error e) {
        torch_last_err = strdup(e.what());
    }

    return nullptr;
}

Tensor THSTensor_randint_out(const Generator gen, const int64_t low, const int64_t high, const int64_t* sizes, const int length, const Tensor out)
{
    CATCH_TENSOR(gen == nullptr ? torch::randint_out(*out, low, high, at::ArrayRef<int64_t>(sizes, length)) : torch::randint_out(*out, low, high, at::ArrayRef<int64_t>(sizes, length), *gen));
}

Tensor THSTensor_randint_like(
    const Tensor input,
    const int64_t low,
    const int64_t high,
    const int8_t scalar_type,
    const int device_type, const int device_index,
    const bool requires_grad)
{
    auto options = at::TensorOptions()
        .dtype(at::ScalarType(scalar_type))
        .device(c10::Device((c10::DeviceType)device_type, (c10::DeviceIndex)device_index))
        .requires_grad(requires_grad);

    CATCH_TENSOR(torch::randint_like(*input, low, high, options));
}

Tensor THSTensor_randn(
    const Generator gen,
    const int64_t* sizes,
    const int length,
    const int8_t scalar_type,
    const int device_type, const int device_index,
    const bool requires_grad)
{
    auto options = at::TensorOptions()
        .dtype(at::ScalarType(scalar_type))
        .device(c10::Device((c10::DeviceType)device_type, (c10::DeviceIndex)device_index))
        .requires_grad(requires_grad);

    CATCH_TENSOR(gen == nullptr ? torch::randn(at::ArrayRef<int64_t>(sizes, length), options) : torch::randn(at::ArrayRef<int64_t>(sizes, length), *gen, options));
}

Tensor THSTensor_randn_out(const Generator gen, const int64_t* sizes, const int length, const Tensor out)
{
    CATCH_TENSOR(gen == nullptr ? torch::randn_out(*out, at::ArrayRef<int64_t>(sizes, length)) : torch::randn_out(*out, at::ArrayRef<int64_t>(sizes, length), *gen));
}

Tensor THSTensor_randn_like(
    const Tensor input,
    const int8_t scalar_type,
    const int device_type, const int device_index,
    const bool requires_grad)
{
    auto options = at::TensorOptions()
        .dtype(at::ScalarType(scalar_type))
        .device(c10::Device((c10::DeviceType)device_type, (c10::DeviceIndex)device_index))
        .requires_grad(requires_grad);

    CATCH_TENSOR(torch::randn_like(*input, options));
}


Tensor THSTensor_random_(Tensor tensor, double low, double high, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? tensor->random_(low, high) : tensor->random_(low, high, *gen));
}


Tensor THSTensor_randperm(
    const Generator gen,
    const int64_t n,
    const int8_t scalar_type,
    const int device_type, const int device_index,
    const bool requires_grad)
{
    Tensor tensor;
    CATCH(
        auto options =
        at::TensorOptions()
        .dtype(at::ScalarType(scalar_type))
        .device(c10::Device((c10::DeviceType)device_type, (c10::DeviceIndex)device_index))
        .requires_grad(requires_grad);

        tensor = new torch::Tensor(gen == nullptr ? torch::randperm(n, options) : torch::randperm(n, *gen, options));
    )
        return tensor;
}

Tensor THSTensor_randperm_out(const Generator gen, const int64_t n, const Tensor out)
{
    CATCH_TENSOR(gen == nullptr ? torch::randperm_out(*out, n) : torch::randperm_out(*out, n, *gen));
}

Tensor THSInit_kaiming_normal_(Tensor tensor, double a, const int64_t mode, const int64_t nonlinearity)
{
    CATCH_TENSOR(torch::nn::init::kaiming_normal_(*tensor, a, mode == 0 ? torch::nn::init::FanModeType(torch::kFanIn) : torch::nn::init::FanModeType(torch::kFanOut), get_nl_type(nonlinearity)))
}

Tensor THSInit_kaiming_uniform_(Tensor tensor, double a, const int64_t mode, const int64_t nonlinearity)
{
    CATCH_TENSOR(torch::nn::init::kaiming_uniform_(*tensor, a, mode == 0 ? torch::nn::init::FanModeType(torch::kFanIn) : torch::nn::init::FanModeType(torch::kFanOut), get_nl_type(nonlinearity)))
}

Tensor THSTensor_uniform_(Tensor tensor, double low, double high, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? tensor->uniform_(low, high) : tensor->uniform_(low, high, *gen));
}

Tensor THSTensor_sample_dirichlet_(Tensor tensor, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? torch::_sample_dirichlet(*tensor) : torch::_sample_dirichlet(*tensor, *gen));
}

Tensor THSTensor_standard_gamma_(Tensor tensor, const Generator gen)
{
    CATCH_TENSOR(gen == nullptr ? torch::_standard_gamma(*tensor)  : torch::_standard_gamma(*tensor, *gen));
}

Tensor THSInit_uniform_(Tensor tensor, double low, double high)
{
    CATCH_TENSOR(torch::nn::init::uniform_(*tensor, low, high))
}

Tensor THSInit_xavier_normal_(Tensor tensor, double gain)
{
    CATCH_TENSOR(torch::nn::init::xavier_normal_(*tensor, gain))
}

Tensor THSInit_xavier_uniform_(Tensor tensor, double gain)
{
    CATCH_TENSOR(torch::nn::init::xavier_uniform_(*tensor, gain))
}

