#ifndef PARTICLE_SET_HPP
#define PARTICLE_SET_HPP

#include "particle.hpp"

#include <memory>
#include <vector>

namespace muse_amcl {
template<typename T, T Particle::*Member>
class ParticleMemberIterator : public std::iterator<std::random_access_iterator_tag, T>
{
    Particle *data;

    using parent = std::iterator<std::random_access_iterator_tag, T>;
    using iterator = typename parent::iterator;
    using reference = typename parent::reference;

public:
    explicit ParticleMemberIterator(Particle *_begin) :
        data(_begin)
    {
    }

    iterator& operator++()
    {
        ++data;
        return *this;
    }

    bool operator ==(const ParticleMemberIterator<T, Member> &_other) const
    {
        return data == _other.data;
    }

    bool operator !=(const ParticleMemberIterator<T, Member> &_other) const
    {
        return !(*this == _other);
    }

    reference operator *() const
    {
        return (data->*Member);
    }
};

class ParticleSet
{
public:
    typedef std::shared_ptr<ParticleSet> Ptr;
    typedef std::shared_ptr<ParticleSet const> ConstPtr;

    template<typename T, T Particle::*Member>
    class ParticleDecorator {
    public:
        ParticleDecorator(ParticleSet &_set) :
            set(_set)
        {
        }

        ParticleMemberIterator<T, Member> begin()
        {
            return ParticleMemberIterator<T, Member>(&set.samples.front());
        }

        ParticleMemberIterator<T, Member> end() {
            return ParticleMemberIterator<T, Member>(&set.samples.back());
        }
    private:
        ParticleSet& set;
    };

    using PoseIterator = ParticleDecorator<Particle::PoseType, &Particle::pose_>;
    using WeightIterator = ParticleDecorator<Particle::WeightType, &Particle::weight_>;
    using Particles = std::vector<Particle>;

    ParticleSet(const std::size_t _sample_size) :
        samples(_sample_size)
    {
    }

    PoseIterator getPoses()
    {
        return PoseIterator(*this);
    }

    WeightIterator getWeights()
    {
        return WeightIterator(*this);
    }

    Particles & getParticles()
    {
        return samples;
    }

    void resize(const std::size_t _sample_size)
    {
        samples.resize(_sample_size);
    }

    void reserve(const std::size_t _sample_size)
    {
        samples.reserve(_sample_size);
    }

    void emplace_back(const Particle &_sample)
    {
        samples.emplace_back(_sample);
    }

    std::size_t minimumSampleSize() const
    {
        return minimum_sample_size_;
    }

    std::size_t maximumSampleSize() const
    {
        return maximum_sample_size_;
    }

    std::size_t currenSampleSize() const
    {
        return samples.size();
    }

private:
    std::size_t minimum_sample_size_;
    std::size_t maximum_sample_size_;
    Particles samples;

};
}

#endif /* PARTICLE_SET_HPP */
